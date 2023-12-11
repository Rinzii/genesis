// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/device.hpp"
#include "gen/rendering/utils/exceptions.hpp"
#include "gen/windowing/window.hpp"

#include <GLFW/glfw3.h>

#include <set>
#include <gen/rendering/vk/devices/logicalDevice.hpp>

namespace gen
{
	namespace
	{
		static const std::vector<const char *> mandatoryDeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
#ifdef GEN_PLATFORM_APPLE
			VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
#endif
		};

		u32 findQueueFamily(const vk::PhysicalDevice & gpu, const vk::SurfaceKHR & surface)
		{
			u32 indices{};

			auto queueFamilies = gpu.getQueueFamilyProperties();

			int index{0};
			for (const auto & queueFamily : queueFamilies)
			{
				bool const hasGraphicsFlag = (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlagBits(0);
				bool const hasTransferFlag = (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) != vk::QueueFlagBits(0);

				if (hasGraphicsFlag && hasTransferFlag &&
					gpu.getSurfaceSupportKHR(static_cast<u32>(index), surface)) // NOLINT(readability-implicit-bool-conversion)
				{
					indices = static_cast<u32>(index);
					break;
				}

				index++;
			}

			return indices;
		}

		bool doesGpuSupportRequiredExtensions(const vk::PhysicalDevice & gpu, const std::vector<const char *> & requiredExtensions)
		{
			if (requiredExtensions.empty()) { return true; }

			auto availableExtensions = gpu.enumerateDeviceExtensionProperties();

			for (const auto & required : requiredExtensions)
			{
				bool failedToFindExt{true};
				for (auto & available : availableExtensions)
				{
					if (std::strcmp(required, available.extensionName) == 0)
					{
						failedToFindExt = false;
						break;
					}
				}

				if (failedToFindExt) { return false; }
			}

			return true;
		}

		Gpu selectIdealGpu(vk::Instance instance, vk::SurfaceKHR surface, const std::vector<const char *> & requiredExtensions)
		{
			auto availablePhysDevices = instance.enumeratePhysicalDevices();

			if (availablePhysDevices.empty()) { throw gen::VulkanException("Could not find any GPUs!"); }

			Gpu idealGpu{};

			// Attempt to find a discrete GPU that can render to the surface.
			for (auto availablePhysDevice : availablePhysDevices)
			{
				auto properties = availablePhysDevice.getProperties();
				if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
				{
					if (doesGpuSupportRequiredExtensions(availablePhysDevice, requiredExtensions))
					{
						idealGpu.physicalDevice			  = availablePhysDevice;
						idealGpu.properties				  = properties;
						idealGpu.memoryProperties		  = availablePhysDevice.getMemoryProperties();
						idealGpu.features				  = availablePhysDevice.getFeatures();
						idealGpu.graphicsQueueFamilyIndex = findQueueFamily(availablePhysDevice, surface);
						idealGpu.queueFamilyProperties	  = availablePhysDevice.getQueueFamilyProperties()[idealGpu.graphicsQueueFamilyIndex];

						return idealGpu;
					}
				}
			}

			// If we didn't find a discrete GPU, we'll just use the first device in the list.
			idealGpu.physicalDevice			  = availablePhysDevices.front();
			idealGpu.properties				  = availablePhysDevices.front().getProperties();
			idealGpu.memoryProperties		  = availablePhysDevices.front().getMemoryProperties();
			idealGpu.features				  = availablePhysDevices.front().getFeatures();
			idealGpu.graphicsQueueFamilyIndex = findQueueFamily(availablePhysDevices.front(), surface);
			idealGpu.queueFamilyProperties	  = availablePhysDevices.front().getQueueFamilyProperties()[idealGpu.graphicsQueueFamilyIndex];

			return idealGpu;
		}
	} // namespace

	Device::Device(vk::Instance instance, const std::vector<const char *> & requiredExtensions)
	{
		// Append mandatory extensions and required extensions together to simplify the extension checking.
		auto requestedExtensions = mandatoryDeviceExtensions;
		requestedExtensions.insert(requestedExtensions.end(), requiredExtensions.begin(), requiredExtensions.end());

		m_surface = createSurface(instance);

		m_gpu = selectIdealGpu(instance, m_surface.get(), requestedExtensions);

		// If we reach this point then the requested extensions are supported and considered enabled.
		m_enabledExtensions = requestedExtensions;

		// Begin creation of the logical device.
		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		float const queuePriority				= 1.0F;
		std::set<u32> const uniqueQueueFamilies = {m_gpu.graphicsQueueFamilyIndex};

		for (u32 const queueFamily : uniqueQueueFamilies)
		{
			vk::DeviceQueueCreateInfo const queueCreateInfo({}, queueFamily, 1, &queuePriority);
			queueCreateInfos.push_back(queueCreateInfo);
		}

		vk::DeviceCreateInfo createInfo{};
		createInfo.queueCreateInfoCount	   = static_cast<u32>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos	   = queueCreateInfos.data();
		createInfo.enabledExtensionCount   = static_cast<u32>(m_enabledExtensions.size());
		createInfo.ppEnabledExtensionNames = m_enabledExtensions.data();

		// Enable dynamic rendering extension.
		// Currently, we mandate dynamic rendering. Maybe this will change down the line, but for now it's a requirement.
		auto dynamicRenderingFeatures = vk::PhysicalDeviceDynamicRenderingFeatures{vk::True};
		createInfo.pNext			  = &dynamicRenderingFeatures;

		// Enable device synchronization extension.
		auto deviceSyncFeatures	 = vk::PhysicalDeviceSynchronization2FeaturesKHR{vk::True};
		deviceSyncFeatures.pNext = &deviceSyncFeatures;

		m_handle = m_gpu.physicalDevice.createDeviceUnique(createInfo);

		// Currently we only support a single queue. Likely will not support multi-queue.
		m_graphicsQueue = m_handle->getQueue(m_gpu.graphicsQueueFamilyIndex, 0);
	}

	Device::~Device()
	{
		m_handle->waitIdle();
	}

	void Device::submit(const vk::CommandBuffer & cmd, vk::Fence fence) const
	{
		auto lock = std::scoped_lock{m_queueMutex};
		vk::SubmitInfo const submitInfo{{}, {}, cmd};
		m_graphicsQueue.submit(submitInfo, fence);
	}

	vk::Result Device::present(const vk::PresentInfoKHR & presentInfo) const
	{
		auto lock = std::scoped_lock{m_queueMutex};
		return m_graphicsQueue.presentKHR(presentInfo);
	}

	u32 Device::getMemoryType(u32 typeBits, vk::MemoryPropertyFlags properties, vk::Bool32 * memTypeFound) const
	{
		for (u32 i = 0; i < m_gpu.memoryProperties.memoryTypeCount; i++)
		{
			if ((typeBits & 1) == 1)
			{
				if ((m_gpu.memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					if (memTypeFound != nullptr) { *memTypeFound = true; } // NOLINT(readability-implicit-bool-conversion)
					return i;
				}
			}
			typeBits >>= 1;
		}

		if (memTypeFound != nullptr)
		{
			*memTypeFound = false; // NOLINT(readability-implicit-bool-conversion)
			return 0;
		}

		throw gen::VulkanException("Could not find a matching memory type!");
	}

	vk::UniqueSurfaceKHR Device::createSurface(const vk::Instance instance)
	{
		VkSurfaceKHR surface{};
		VkResult const err = glfwCreateWindowSurface(instance, Window::self().getHandle(), nullptr, &surface);
		if (err != VK_SUCCESS) { throw gen::VulkanException(std::format("Failed to create window surface: {}", vk::to_string(vk::Result(err)))); }

		return vk::UniqueSurfaceKHR(surface, instance);
	}

} // namespace gen