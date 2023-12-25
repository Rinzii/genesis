// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/logger/log.hpp"
#include "gen/rendering/utils/debugUtils.hpp"
#include "gen/rendering/utils/exceptions.hpp"
#include "gen/rendering/utils/vkHelpers.hpp"
#include "gen/rendering/vk/devices/logicalDevice.hpp"
#include "gen/system/types.hpp"

#include "gen/rendering/renderWindow.hpp"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

namespace gen
{
	namespace
	{
		const Logger s_logger{"vulkan"};
		constexpr auto defaultQueuePriority_v = 1.0F;

		Gpu populateGpuInfo(vk::PhysicalDevice physicalDevice)
		{
			Gpu gpu{};
			gpu.physicalDevice		  = physicalDevice;
			gpu.type				  = gpu.physicalDevice.getProperties().deviceType;
			gpu.properties			  = gpu.physicalDevice.getProperties();
			gpu.features			  = gpu.physicalDevice.getFeatures();
			gpu.memoryProperties	  = gpu.physicalDevice.getMemoryProperties();
			gpu.queueFamilyProperties = gpu.physicalDevice.getQueueFamilyProperties();
			return gpu;
		}

		vk::Result doesGpuSupportSurface(const vk::PhysicalDevice & gpu, const vk::SurfaceKHR & surface)
		{
			vk::SurfaceCapabilitiesKHR surfaceCapabilities{};
			vk::Result const result = gpu.getSurfaceCapabilitiesKHR(surface, &surfaceCapabilities);
			return result;
		}

		u32 findGraphicsQueueFamilyIndex(const vk::PhysicalDevice & pDevice, const vk::SurfaceKHR & surface)
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

		DeviceInfo buildDeviceInfo(vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR & surface)
		{

			vk::PhysicalDeviceProperties const properties = physicalDevice.getProperties();

			vk::PhysicalDeviceFeatures const features = physicalDevice.getFeatures();

			vk::PhysicalDeviceMemoryProperties const memoryProperties = physicalDevice.getMemoryProperties();

			vk::DeviceSize totalDeviceMemory{0};
			for (u32 i = 0; i < memoryProperties.memoryHeapCount; i++)
			{
				if (memoryProperties.memoryHeaps[i].flags & vk::MemoryHeapFlagBits::eDeviceLocal) { totalDeviceMemory += memoryProperties.memoryHeaps[i].size; }
			}

			if (surface != nullptr)
			{
				if (doesGpuSupportSurface(physicalDevice, surface) != vk::Result::eSuccess) { throw VulkanException("GPU does not support surface."); }
			}

			auto familyIndex = findGraphicsQueueFamilyIndex(physicalDevice, surface);

			vk::Bool32 presentSupport{vk::True};
			if (surface != nullptr) { presentSupport = physicalDevice.getSurfaceSupportKHR(familyIndex, surface); }

			const auto extensions = physicalDevice.enumerateDeviceExtensionProperties();

			Gpu const gpu{
				.physicalDevice			  = physicalDevice,
				.type					  = properties.deviceType,
				.properties				  = properties,
				.features				  = features,
				.memoryProperties		  = memoryProperties,
				.queueFamilyProperties	  = physicalDevice.getQueueFamilyProperties(),
				.graphicsQueueFamilyIndex = familyIndex,
				.presentSupport			  = static_cast<bool>(presentSupport)};

			return DeviceInfo{.name = properties.deviceName, .gpu = gpu, .totalDeviceLocalMemory = totalDeviceMemory, .extensions = extensions};
		}

		std::vector<DeviceInfo> getAvailableDevices(vk::Instance instance, const vk::SurfaceKHR & surface)
		{
			auto availablePhysicalDevices = instance.enumeratePhysicalDevices();
			if (availablePhysicalDevices.empty()) { throw VulkanException("Failed to find GPUs with Vulkan support!"); }

			std::vector<DeviceInfo> devices{};
			devices.reserve(availablePhysicalDevices.size());

			for (auto availablePhysicalDevice : availablePhysicalDevices) { devices.push_back(buildDeviceInfo(availablePhysicalDevice, surface)); }

			return devices;
		}

		bool doesGpuSupportExtensions(vk::PhysicalDevice physicalDevice, std::span<const char * const> requiredExtensions)
		{
			auto availableExtensions = physicalDevice.enumerateDeviceExtensionProperties();

			std::unordered_set<std::string> requiredExtensionsSet(requiredExtensions.begin(), requiredExtensions.end());

			for (const auto & extension : availableExtensions)
			{
				requiredExtensionsSet.erase(extension.extensionName);
				if (requiredExtensionsSet.empty())
				{
					return true; // All required extensions are supported, early exit.
				}
			}

			return false; // Some required extensions are not supported.
		}

		bool isGpuSuitable(
			const Gpu & gpu,
			const vk::SurfaceKHR & surface,
			std::span<const char * const> requiredExtensions,
			const vk::PhysicalDeviceFeatures & requiredFeatures)
		{
			// Check if the device is a usable GPU
			if (gpu.properties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
			{
				if (gpu.properties.deviceType != vk::PhysicalDeviceType::eIntegratedGpu) { return false; }
			}

			if (doesGpuSupportSurface(gpu.physicalDevice, surface) != vk::Result::eSuccess) { return false; }

			if (!doesGpuSupportExtensions(gpu.physicalDevice, requiredExtensions)) { return false; }

			if (gpu.features != requiredFeatures) { return false; }

			return true;
		}

		u32 rateGpu(const DeviceInfo & deviceInfo)
		{
			switch (deviceInfo.gpu.type)
			{
			case vk::PhysicalDeviceType::eDiscreteGpu: return 2;
			case vk::PhysicalDeviceType::eIntegratedGpu: return 1;
			default: return 0;
			}
		}

		bool compareGpus(
			const DeviceInfo & rhs,
			const DeviceInfo & lhs,
			const vk::SurfaceKHR surface,
			const vk::PhysicalDeviceFeatures & requiredFeatures,
			std::span<const char * const> requiredExtensions)
		{
			if (!isGpuSuitable(rhs.gpu, surface, requiredExtensions, requiredFeatures)) { return true; }
			if (!isGpuSuitable(lhs.gpu, surface, requiredExtensions, requiredFeatures)) { return false; }

			if (rateGpu(lhs) > rateGpu(rhs)) { return true; }
			if (rateGpu(lhs) < rateGpu(rhs)) { return false; }

			return lhs.totalDeviceLocalMemory >= rhs.totalDeviceLocalMemory;
		}

	} // namespace

	Device::Device(
		PhysicalDevice & gpu,
		std::span<const char * const> requiredExtensions,
		const vk::PhysicalDeviceFeatures & requiredFeatures,
		const vk::PhysicalDeviceFeatures & optionalFeatures,
		std::optional<u32> selectSpecificGpuIndex)
	{
	}

	Device::~Device()
	{
	}

	Device::Device(
		vk::Instance instance,
		vk::SurfaceKHR surface,
		std::span<const char * const> requiredExtensions,
		const vk::PhysicalDeviceFeatures & requiredFeatures,
		const vk::PhysicalDeviceFeatures & optionalFeatures,
		std::optional<u32> selectSpecificGpuIndex)
	{

		m_gpus = getAvailableDevices(instance, surface);
		// Make copy of deviceInfos, so we can pass it to selectBestPhysicalDevice.

		if (m_gpus.empty()) { throw VulkanException("No GPUs were found!"); }

		if (selectSpecificGpuIndex.has_value())
		{
			if (selectSpecificGpuIndex.value() >= m_gpus.size()) { throw VulkanException("Invalid GPU index specified!"); }

			m_gpu = m_gpus[selectSpecificGpuIndex.value()];
		}
		else { m_gpu = buildDeviceInfo(selectBestPhysicalDevice(m_gpus, surface, requiredExtensions, requiredFeatures), surface); }

		// Create the logical device.

		m_gpu.gpu.graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex(m_gpu.gpu.physicalDevice, surface);

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};
		queueCreateInfos.reserve(1);

		queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags{}, m_gpu.gpu.graphicsQueueFamilyIndex, 1, &defaultQueuePriority_v);

		vk::DeviceCreateInfo deviceCreateInfo{
			vk::DeviceCreateFlags{},
			static_cast<u32>(queueCreateInfos.size()),
			queueCreateInfos.data(),
			0,
			nullptr,
			static_cast<u32>(requiredExtensions.size()),
			requiredExtensions.data(),
			&requiredFeatures};

		// Create the device handle.
		m_handle = m_gpu.gpu.physicalDevice.createDeviceUnique(deviceCreateInfo);

		// Get the graphics queue handle.
		m_graphicsQueue = m_handle->getQueue(m_gpu.gpu.graphicsQueueFamilyIndex, 0);

		// TODO: Implement optional features.

		m_handle = m_selectedGpu.physicalDevice.createDeviceUnique(deviceCreateInfo);

		// Get the graphics queue handle.
		m_graphicsQueue = m_handle->getQueue(m_selectedGpu.graphicsQueueFamilyIndex, 0);

		// Create the memory allocator.
		VmaAllocatorCreateInfo allocatorCreateInfo{};
		allocatorCreateInfo.physicalDevice = m_selectedGpu.physicalDevice;
		allocatorCreateInfo.device		   = m_handle.get();
		allocatorCreateInfo.instance	   = instance;
		vmaCreateAllocator(&allocatorCreateInfo, &m_allocator);

		vk::ObjectType::
	}

	vk::PhysicalDevice Device::selectBestPhysicalDevice(
		std::vector<DeviceInfo> & gpus,
		vk::SurfaceKHR surface,
		std::span<const char * const> requiredExtensions,
		const vk::PhysicalDeviceFeatures & requiredFeatures)
	{
		if (gpus.empty()) { throw VulkanException("No GPUs were found!"); }

		std::sort(
			gpus.begin(),
			gpus.end(),
			[&](const DeviceInfo & rhs, const DeviceInfo & lhs) { return compareGpus(rhs, lhs, surface, requiredFeatures, requiredExtensions); });

		if (!isGpuSuitable(gpus.front().gpu, surface, requiredExtensions, requiredFeatures)) { throw VulkanException("No suitable GPU was found!"); }

		return gpus.front().gpu.physicalDevice;
	}

	vk::PhysicalDevice Device::selectBestPhysicalDevice(
		const vk::Instance & instance,
		const vk::SurfaceKHR & surface,
		std::span<const char * const> requiredExtensions,
		const vk::PhysicalDeviceFeatures & requiredFeatures)
	{
		auto availablePhysicalDevices = instance.enumeratePhysicalDevices();
		if (availablePhysicalDevices.empty()) { throw VulkanException("Failed to find GPUs with Vulkan support!"); }

		std::vector<DeviceInfo> devices(availablePhysicalDevices.size());
		std::transform(
			availablePhysicalDevices.begin(),
			availablePhysicalDevices.end(),
			devices.begin(),
			[&](const vk::PhysicalDevice & physicalDevice) { return buildDeviceInfo(physicalDevice, surface); });

		return selectBestPhysicalDevice(devices, surface, requiredExtensions, requiredFeatures);
	}

	Gpu Device::selectPhysicalDevice(
		const vk::Instance & instance,
		const vk::SurfaceKHR & surface,
		std::span<const char * const> requiredExtensions,
		const vk::PhysicalDeviceFeatures & requiredFeatures,
		const vk::PhysicalDeviceFeatures & optionalFeatures)
	{
		auto availablePhysicalDevices = instance.enumeratePhysicalDevices();
		if (availablePhysicalDevices.empty()) { throw VulkanException("Failed to find GPUs with Vulkan support!"); }

		for (std::size_t i = 0; i < availablePhysicalDevices.size(); i++)
		{
			Gpu currentGpu{};
			currentGpu.physicalDevice		 = availablePhysicalDevices[i];
			currentGpu.memoryProperties		 = currentGpu.physicalDevice.getMemoryProperties();
			currentGpu.queueFamilyProperties = currentGpu.physicalDevice.getQueueFamilyProperties();
			if (!isGpuSuitable(currentGpu, surface, requiredExtensions, requiredFeatures))
			{
				// This GPU is not suitable, try the next one.
				continue;
			}

			// Report the GPU information to the logger.
			auto aDev = std::stringstream{"vulkan"};
			aDev << "\tDevice [" << i << "] : "
				 << "\n"
				 << "\t\tName: " << availablePhysicalDevices[i].getProperties().deviceName << "\n"
				 << "\t\tType: " << to_string(availablePhysicalDevices[i].getProperties().deviceType) << "\n"
				 << "\t\tAPI Version: " << vk::utils::intToSemver(availablePhysicalDevices[i].getProperties().apiVersion) << "\n"
				 << "\t\tDriver Version: " << vk::utils::intToSemver(availablePhysicalDevices[i].getProperties().driverVersion) << "\n"
				 << "\t\tVendor Name: " << vk::utils::getDriverVendorName(availablePhysicalDevices[i].getProperties().vendorID) << "\n"
				 << "\t\tVendor ID: 0x" << std::hex << availablePhysicalDevices[i].getProperties().vendorID << "\n"
				 << "\t\tDevice ID: 0x" << std::hex << availablePhysicalDevices[i].getProperties().deviceID << "\n";

			GEN_LOG_INFO(s_logger, "Available devices: \n{}", aDev.str());
			return currentGpu;
		}

		// If we get here, no suitable GPU was found. Fall back to the first GPU.
		Gpu fallbackGpu{};
		fallbackGpu.physicalDevice		  = availablePhysicalDevices[0];
		fallbackGpu.properties			  = fallbackGpu.physicalDevice.getProperties();
		fallbackGpu.features			  = fallbackGpu.physicalDevice.getFeatures();
		fallbackGpu.memoryProperties	  = fallbackGpu.physicalDevice.getMemoryProperties();
		fallbackGpu.queueFamilyProperties = fallbackGpu.physicalDevice.getQueueFamilyProperties();
		return fallbackGpu;
	}

} // namespace gen
