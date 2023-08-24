// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/device.hpp"
#include "gen/graphics/graphicsExceptions.hpp"
#include "gen/graphics/vkHelpers.hpp"

#include <sstream>
#include <string>

namespace gen
{
	static const std::vector<const char *> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef GEN_PLATFORM_APPLE
		VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
#endif
	};

	constexpr float desiredQueuePriority_v{1.0F};

	Device::Device(const std::string & appName, const u32 appVersion, const std::string & engineName, const u32 & apiVersion)
	{
		createInstance(appName, appVersion, engineName, apiVersion);
		createSurface();
		selectPhysicalDevice();
		createLogicalDevice();
		m_logger.debug("Device constructed");
	}

	Device::~Device()
	{
		m_device->waitIdle();
		m_logger.debug("Device destroyed");
	}

	void Device::createInstance(const std::string & appName, const u32 appVersion, const std::string & engineName, const u32 & apiVersion)
	{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const dynLoader;
		auto vkGetInstanceProcAddr = dynLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		vk::ApplicationInfo const appInfo(appName.c_str(), appVersion, engineName.c_str(), gen::version_v.getVersion(), apiVersion);

		auto extensionsCount	   = 0U;
		auto * requestedExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
		std::vector<std::string> const requestedExtensionsVec(
			requestedExtensions,
			requestedExtensions + extensionsCount); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		auto enabledExtensions = vk::util::gatherExtensions(
			requestedExtensionsVec
#ifndef GEN_NDEBUG
			,
			vk::enumerateInstanceExtensionProperties()
#endif
		);

		m_instance = vk::createInstanceUnique(vk::util::makeInstanceCreateInfoChain(appInfo, {}, enabledExtensions).get<vk::InstanceCreateInfo>());

		if (!m_instance) { throw gen::vulkan_error("Failed to create instance!"); }

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);
#endif
	}

	void Device::createSurface()
	{
		m_surface = vk::util::createWindowSurface(m_instance.get(), Window::getInstance());
	}

	void Device::selectPhysicalDevice()
	{
		auto availablePhysicalDevices = m_instance.get().enumeratePhysicalDevices();
		if (availablePhysicalDevices.empty()) { throw gen::vulkan_error("Failed to find GPUs with Vulkan support!"); }

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

		auto aDev = std::stringstream{"vulkan"};
		for (std::size_t i = 0; i < availablePhysicalDevices.size(); i++)
		{
			aDev << "\tDevice [" << i << "] : "
				 << "\n"
				 << "\t\tName: " << availablePhysicalDevices[i].getProperties().deviceName << "\n"
				 << "\t\tType: " << to_string(availablePhysicalDevices[i].getProperties().deviceType) << "\n"
				 << "\t\tAPI Version: " << vk::util::intToSemver(availablePhysicalDevices[i].getProperties().apiVersion) << "\n"
				 << "\t\tDriver Version: " << vk::util::intToSemver(availablePhysicalDevices[i].getProperties().driverVersion) << "\n"
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

		m_graphicsQueue = m_device->getQueue(m_gpu.queueFamily, 0);
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

} // namespace gen
