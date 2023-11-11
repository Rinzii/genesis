// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/device.hpp"
#include "gen/graphics/graphicsExceptions.hpp"
#include "gen/graphics/vkHelpers.hpp"

#include <mutex>
#include <sstream>
#include <string>

namespace gen
{
	static const std::vector<const char *> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
#ifdef GEN_PLATFORM_APPLE
		VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
#endif
	};

	constexpr float desiredQueuePriority_v{1.0F};

	Device::Device(vk::Instance instance)
	{
		createSurface(instance);
		selectPhysicalDevice(instance);
		createLogicalDevice();
		m_logger.debug("Device constructed");
	}

	Device::~Device()
	{
		m_device->waitIdle();
		m_logger.debug("Device destroyed");
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

	void Device::createSurface(vk::Instance instance)
	{
		m_surface = vk::utils::createWindowSurface(instance, Window::getInstance());
	}

	void Device::selectPhysicalDevice(vk::Instance instance)
	{
		auto availablePhysicalDevices = instance.enumeratePhysicalDevices();
		if (availablePhysicalDevices.empty()) { throw gen::VulkanException("Failed to find GPUs with Vulkan support!"); }

		// Search for a discrete gpu in our list.
		for (const auto & device : availablePhysicalDevices)
		{
			auto properties = device.getProperties();

			if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
			{
				m_gpu.physicalDevice = device;
				m_gpu.properties	 = properties;
				break;
			}
		}

		// If we didn't find a discrete gpu, we'll just use the first device in the list.
		if (!m_gpu.physicalDevice)
		{
			m_gpu.physicalDevice = availablePhysicalDevices.front();
			m_gpu.properties	 = availablePhysicalDevices.front().getProperties();
		}

		// Populate our memory properties.
		m_gpu.memoryProperties = m_gpu.physicalDevice.getMemoryProperties();

		// Populate our device features.
		m_gpu.deviceFeatures = m_gpu.physicalDevice.getFeatures();

		auto aDev = std::stringstream{"vulkan"};
		for (std::size_t i = 0; i < availablePhysicalDevices.size(); i++)
		{
			aDev << "\tDevice [" << i << "] : "
				 << "\n"
				 << "\t\tName: " << availablePhysicalDevices[i].getProperties().deviceName << "\n"
				 << "\t\tType: " << to_string(availablePhysicalDevices[i].getProperties().deviceType) << "\n"
				 << "\t\tAPI Version: " << vk::utils::intToSemver(availablePhysicalDevices[i].getProperties().apiVersion) << "\n"
				 << "\t\tDriver Version: " << vk::utils::intToSemver(availablePhysicalDevices[i].getProperties().driverVersion) << "\n"
				 << "\t\tVendor ID: 0x" << std::hex << availablePhysicalDevices[i].getProperties().vendorID << "\n"
				 << "\t\tDevice ID: 0x" << std::hex << availablePhysicalDevices[i].getProperties().deviceID << "\n";
		}
		m_logger.info("Found Physical Devices: \n{}\n", aDev.str());
	}

	void Device::createLogicalDevice()
	{
		m_gpu.queueFamily = findQueueFamily(m_gpu.physicalDevice, m_surface.get());

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		std::set<u32> const uniqueQueueFamilies = {m_gpu.queueFamily};

		// Identify features we have available and try to enable them.
		auto enabledFeatures	= vk::PhysicalDeviceFeatures{};
		auto availableFeatures	= m_gpu.physicalDevice.getFeatures();
		enabledFeatures.logicOp = availableFeatures.logicOp;

		for (u32 const queueFamily : uniqueQueueFamilies)
		{
			vk::DeviceQueueCreateInfo const queueCreateInfo({}, queueFamily, 1, &desiredQueuePriority_v);
			queueCreateInfos.push_back(queueCreateInfo);
		}

		vk::DeviceCreateInfo createInfo{};
		createInfo.queueCreateInfoCount	   = static_cast<u32>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos	   = queueCreateInfos.data();
		createInfo.enabledExtensionCount   = static_cast<u32>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.pEnabledFeatures		   = &enabledFeatures;

		// Tell our create info that we want to use dynamic rendering.
		auto dynamicRenderingFeature = vk::PhysicalDeviceDynamicRenderingFeatures{vk::True};
		createInfo.pNext			 = &dynamicRenderingFeature;

		auto pDeviceSyncFeatures	  = vk::PhysicalDeviceSynchronization2FeaturesKHR{vk::True};
		dynamicRenderingFeature.pNext = &pDeviceSyncFeatures;

		m_device = m_gpu.physicalDevice.createDeviceUnique(createInfo);

		m_queue = m_device->getQueue(m_gpu.queueFamily, 0);
	}

	u32 Device::findQueueFamily(const vk::PhysicalDevice & pDevice, const vk::SurfaceKHR & surface)
	{
		u32 indices{};

		auto queueFamilies = pDevice.getQueueFamilyProperties();

		int index{0};
		for (const auto & queueFamily : queueFamilies)
		{
			bool const hasGraphicsFlag = (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlagBits(0);
			bool const hasTransferFlag = (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) != vk::QueueFlagBits(0);

			if (hasGraphicsFlag && hasTransferFlag &&
				pDevice.getSurfaceSupportKHR(static_cast<u32>(index), surface)) // NOLINT(readability-implicit-bool-conversion)
			{
				indices = static_cast<u32>(index);
				break;
			}

			index++;
		}

		return indices;
	}

	bool Device::waitForFence(vk::Fence fence, u64 timeout) const
	{
		return getDevice().waitForFences(fence, vk::True, timeout) == vk::Result::eSuccess;
	}

	bool Device::submit(const vk::SubmitInfo & submitInfo, vk::Fence signal) const
	{
		auto lock = std::scoped_lock{m_queueMutex};
		return getQueue().submit(1, &submitInfo, signal) == vk::Result::eSuccess;
	}

	bool Device::submit2(const vk::SubmitInfo2 & submitInfo, vk::Fence signal) const
	{
		auto lock = std::scoped_lock{m_queueMutex};
		return getQueue().submit2(1, &submitInfo, signal) == vk::Result::eSuccess;
	}

	bool Device::submitAndPresent(const vk::SubmitInfo & submitInfo, vk::Fence signal, const vk::PresentInfoKHR & presentInfo) const
	{
		auto lock						   = std::scoped_lock{m_queueMutex};
		[[maybe_unused]] auto const result = getQueue().submit(1, &submitInfo, signal);
		return getQueue().presentKHR(&presentInfo) == vk::Result::eSuccess;
	}

	bool Device::submit2AndPresent(const vk::SubmitInfo2 & submitInfo, vk::Fence signal, const vk::PresentInfoKHR & presentInfo) const
	{
		auto lock						   = std::scoped_lock{m_queueMutex};
		[[maybe_unused]] auto const result = getQueue().submit2(1, &submitInfo, signal);
		return getQueue().presentKHR(&presentInfo) == vk::Result::eSuccess;
	}

} // namespace gen
