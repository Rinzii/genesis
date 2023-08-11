// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/device.hpp"
#include "graphics/graphicsExceptions.hpp"
#include "graphics/vkHelpers.hpp"
#include "logger/log.hpp"

#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include <format>
#include <numeric>
#include <sstream>
#include <string>

namespace gen
{
	static const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	GraphicsDevice::GraphicsDevice(const Window & window, std::string const & appName)
	{
		// TODO: Allow for the vulkan api to be specified in a config file or through command line arguments.
		// We could also dynamically check for the highest supported version of the vulkan api, but that feels outside the scope of this project.
		createInstance(appName, "Genesis Engine", VK_API_VERSION_1_3);
		createSurface(window);
		pickPhysicalDevice();
		createLogicalDevice();

		gen::logger::info("vulkan", "GraphicsDevice constructed");
	}

	GraphicsDevice::~GraphicsDevice()
	{
		gen::logger::info("vulkan", "GraphicsDevice destructed");
	}

	void GraphicsDevice::createInstance(const std::string & appName, const std::string & engineName, const gen::u32 & apiVersion)
	{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const dynLoader;
		auto vkGetInstanceProcAddr = dynLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		// TODO: Allow for the application to be able to specify its version itself. Instead of just using the engine version.
		vk::ApplicationInfo const appInfo(appName.c_str(), gen::version_v.getVersion(), engineName.c_str(), gen::version_v.getVersion(), apiVersion);

		auto extensionsCount	   = 0U;
		auto * requestedExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
		std::vector<std::string> const requestedExtensionsVec(requestedExtensions,
															  requestedExtensions + extensionsCount); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		auto enabledExtensions = vk::util::gatherExtensions(requestedExtensionsVec
#ifndef GEN_NDEBUG
															,
															vk::enumerateInstanceExtensionProperties()
#endif
		);

		gen::logger::debug("vulkan",
						   std::format("Enabled extensions: \n\t{}", std::accumulate(enabledExtensions.begin(), enabledExtensions.end(), std::string(),
																					 [](const std::string & acc, const std::string & ext)
																					 { return acc.empty() ? ext : acc + ", \n\t" + ext; })));

		m_instance = vk::createInstanceUnique(vk::util::makeInstanceCreateInfoChain(appInfo, {}, enabledExtensions).get<vk::InstanceCreateInfo>());

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);
#endif
	}

	void GraphicsDevice::createSurface(const Window & window)
	{
		m_surface = vk::util::createWindowSurface(m_instance.get(), window);
		gen::logger::debug("vulkan", "Created surface");
	}

	void GraphicsDevice::pickPhysicalDevice()
	{
		// TODO: Add a weighting system to pick the best physical device.
		// TODO: Add a way to specify the physical device to use if not using the default.
		auto availablePhysicalDevices = m_instance->enumeratePhysicalDevices();
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
				 << "\t\tAPI Version: " << availablePhysicalDevices[i].getProperties().apiVersion << "\n"
				 << "\t\tDriver Version: " << availablePhysicalDevices[i].getProperties().driverVersion << "\n"
				 << "\t\tVendor ID: " << availablePhysicalDevices[i].getProperties().vendorID << "\n"
				 << "\t\tDevice ID: " << availablePhysicalDevices[i].getProperties().deviceID << "\n";
		}
		gen::logger::debug("vulkan", std::format("Found Physical Devices: \n{}\n", aDev.str()));
	}

	void GraphicsDevice::createLogicalDevice()
	{
		m_gpu.queueFamily = findQueueFamilies(m_gpu.physicalDevice, m_surface.get());

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		std::set<u32> const uniqueQueueFamilies = {m_gpu.queueFamily};

		float const queuePriority = 1.0F;
		for (u32 const queueFamily : uniqueQueueFamilies)
		{
			vk::DeviceQueueCreateInfo const queueCreateInfo({}, queueFamily, 1, &queuePriority);
			queueCreateInfos.push_back(queueCreateInfo);
		}

		vk::DeviceCreateInfo const createInfo({}, static_cast<u32>(queueCreateInfos.size()), queueCreateInfos.data());

		m_device = m_gpu.physicalDevice.createDeviceUnique(createInfo);

		m_graphicsQueue = m_device->getQueue(m_gpu.queueFamily, 0);
	}

	u32 GraphicsDevice::findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface)
	{

		u32 indices{};

		auto queueFamilies = device.getQueueFamilyProperties();

		auto qProp = std::stringstream{"vulkan"};
		for (std::size_t i = 0; i < queueFamilies.size(); i++)
		{
			qProp << "\t"
				  << "Queue Family [" << i << "] :\n"
				  << "\t\t\tQueue Flags: " << to_string(queueFamilies[i].queueFlags) << "\n"
				  << "\t\t\tQueue Count: " << queueFamilies[i].queueCount << "\n"
				  << "\t\t\tTimestamp Valid Bits: " << queueFamilies[i].timestampValidBits << "\n"
				  << "\t\t\tMin Image Transfer Granularity: width " << queueFamilies[i].minImageTransferGranularity.width << ", height "
				  << queueFamilies[i].minImageTransferGranularity.height << " , depth " << queueFamilies[i].minImageTransferGranularity.depth << "\n";
		}
		gen::logger::debug("vulkan", std::format("Found Queue Family Properties: \n{}\n", qProp.str()));

		int index{0};
		for (const auto & queueFamily : queueFamilies)
		{
			bool const hasGraphicsFlag = (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlagBits(0);
			bool const hasTransferFlag = (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) != vk::QueueFlagBits(0);

			if (hasGraphicsFlag && hasTransferFlag &&
				device.getSurfaceSupportKHR(static_cast<u32>(index), surface)) // NOLINT(readability-implicit-bool-conversion)
			{
				indices = static_cast<u32>(index);
				break;
			}

			index++;
		}

		gen::logger::debug("vulkan", std::format("Selected graphics queue family: {}", index));

		return indices;
	}

} // namespace gen
