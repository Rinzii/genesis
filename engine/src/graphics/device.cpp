// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/device.hpp"
#include "graphics/graphicsExceptions.hpp"
#include "graphics/vkHelpers.hpp"
#include "logger/log.hpp"

#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include <numeric>
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
	constexpr auto desiredSrgbFormats_v				  = std::array{vk::Format::eB8G8R8A8Srgb, vk::Format::eR8G8B8A8Srgb};
	constexpr vk::PresentModeKHR desiredPresentMode_v = {vk::PresentModeKHR::eMailbox};

	GraphicsDevice::GraphicsDevice(const Window & window, std::string const & appName)
	{
		// TODO: Allow for the vulkan api to be specified in a config file or through command line arguments.
		// We could also dynamically check for the highest supported version of the vulkan api, but that feels outside the scope of this project.
		createInstance(appName, "Genesis Engine", VK_API_VERSION_1_3);
		createSurface(window);
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain(window);
		createImageViews();

		m_logger.info("GraphicsDevice constructed");
	}

	GraphicsDevice::~GraphicsDevice()
	{
		m_device->waitIdle();
		m_logger.info("GraphicsDevice destructed");
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

		m_logger.debug(
			"Enabled extensions: \n\t{}",
			std::accumulate(
				enabledExtensions.begin(),
				enabledExtensions.end(),
				std::string(),
				[](const std::string & acc, const std::string & ext) { return acc.empty() ? ext : acc + ", \n\t" + ext; }));

		m_instance = vk::createInstanceUnique(vk::util::makeInstanceCreateInfoChain(appInfo, {}, enabledExtensions).get<vk::InstanceCreateInfo>());

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);
#endif
	}

	void GraphicsDevice::createSurface(const Window & window)
	{
		m_surface = vk::util::createWindowSurface(m_instance.get(), window);
		m_logger.debug("Created surface");
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
				 << "\t\tAPI Version: " << vk::util::intToSemver(availablePhysicalDevices[i].getProperties().apiVersion) << "\n"
				 << "\t\tDriver Version: " << vk::util::intToSemver(availablePhysicalDevices[i].getProperties().driverVersion) << "\n"
				 << "\t\tVendor ID: 0x" << std::hex << availablePhysicalDevices[i].getProperties().vendorID << "\n"
				 << "\t\tDevice ID: 0x" << std::hex << availablePhysicalDevices[i].getProperties().deviceID << "\n";
		}
		m_logger.debug("Found Physical Devices: \n{}\n", aDev.str());
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

		vk::DeviceCreateInfo const createInfo(
			{},
			static_cast<u32>(queueCreateInfos.size()),
			queueCreateInfos.data(),
			0,		 // ignored by spec
			nullptr, // ignored by spec
			static_cast<u32>(deviceExtensions.size()),
			deviceExtensions.data(),
			nullptr // to be used later
		);

		m_device = m_gpu.physicalDevice.createDeviceUnique(createInfo);

		// display all device extensions using m_device
		auto availableExtensions = m_gpu.physicalDevice.enumerateDeviceExtensionProperties();
		auto aExt				 = std::stringstream{"vulkan"};
		for (std::size_t i = 0; i < availableExtensions.size(); i++)
		{
			aExt << "\tExtension [" << i << "]"
				 << " | Name: " << availableExtensions[i].extensionName << " | Spec Ver: " << availableExtensions[i].specVersion << "\n";
		}
		m_logger.debug("Found Device Extensions: \n{}\n", aExt.str());

		m_graphicsQueue = m_device->getQueue(m_gpu.queueFamily, 0);
	}

	void GraphicsDevice::createSwapChain(const Window & window)
	{
		m_swapChainSupport = querySwapChainSupport(m_gpu.physicalDevice, m_surface.get());
		assert(!m_swapChainSupport.availableFormats.empty());

		m_swapChainSupport.selectedFormat = chooseSwapSurfaceFormat(m_swapChainSupport.availableFormats);

		vk::Extent2D swapchainExtent{};
		int width{}, height{}; // NOLINT
		glfwGetFramebufferSize(window.getHandle(), &width, &height);

		if (m_swapChainSupport.capabilities.currentExtent.width == std::numeric_limits<u32>::max())
		{
			// If the surface size is undefined, the size is set to the size of the images requested.
			swapchainExtent.width =
				std::clamp(static_cast<u32>(width), m_swapChainSupport.capabilities.minImageExtent.width, m_swapChainSupport.capabilities.maxImageExtent.width);
			swapchainExtent.height = std::clamp(
				static_cast<u32>(height), m_swapChainSupport.capabilities.minImageExtent.height, m_swapChainSupport.capabilities.maxImageExtent.height);
		}
		else
		{
			// If the surface size is defined, the swap chain size must match
			swapchainExtent = m_swapChainSupport.capabilities.currentExtent;
		}

		auto maxImageCount = m_swapChainSupport.capabilities.maxImageCount;

		if (maxImageCount == 0) { maxImageCount = std::numeric_limits<u32>::max(); }

		m_swapChainInfo = vk::SwapchainCreateInfoKHR(
			vk::SwapchainCreateFlagsKHR(),
			m_surface.get(),
			std::clamp(3U, m_swapChainSupport.capabilities.minImageCount, maxImageCount),
			m_swapChainSupport.selectedFormat.format,
			m_swapChainSupport.selectedFormat.colorSpace,
			swapchainExtent,
			1,
			vk::ImageUsageFlagBits::eColorAttachment,
			vk::SharingMode::eExclusive,
			{},
			vk::SurfaceTransformFlagBitsKHR::eIdentity,
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			m_swapChainSupport.selectedPresentMode,
			true, // NOLINT(readability-implicit-bool-conversion)
			nullptr);

		m_swapChain = m_device->createSwapchainKHRUnique(m_swapChainInfo);
	}

	void GraphicsDevice::createImageViews()
	{
		m_swapChainImages = m_device->getSwapchainImagesKHR(m_swapChain.get());

		m_swapChainImageViews.reserve(m_swapChainImages.size());

		for (auto image : m_swapChainImages)
		{
			vk::ImageViewCreateInfo const imageViewCreateInfo(
				vk::ImageViewCreateFlags(),
				image,
				vk::ImageViewType::e2D,
				m_swapChainSupport.selectedFormat.format,
				vk::ComponentMapping{vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA},
				vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
			m_swapChainImageViews.push_back(m_device->createImageViewUnique(imageViewCreateInfo));
		}
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
		m_logger.debug("Found Queue Family Properties: \n{}\n", qProp.str());

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

		m_logger.debug("Selected graphics queue family: {}", index);

		return indices;
	}

	SwapChainSupportDetails GraphicsDevice::querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface)
	{
		SwapChainSupportDetails details;
		details.capabilities		  = device.getSurfaceCapabilitiesKHR(surface);
		details.availableFormats	  = device.getSurfaceFormatsKHR(surface);
		details.availablePresentModes = device.getSurfacePresentModesKHR(surface);
		details.selectedPresentMode	  = chooseSwapPresentMode(details.availablePresentModes, desiredPresentMode_v);
		return details;
	}

	vk::SurfaceFormatKHR GraphicsDevice::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> & availableFormats)
	{
		for (const auto & availableFormat : availableFormats)
		{
			if (availableFormat.colorSpace != vk::ColorSpaceKHR::eSrgbNonlinear) { continue; }

			if (std::ranges::find(desiredSrgbFormats_v, availableFormat.format) != desiredSrgbFormats_v.end()) { return availableFormat; }
		}

		// Currently we only support sRGB formats, so if we can't find one, we'll just throw an error.
		throw gen::vulkan_error("Failed to find suitable surface format that supports SRGB!");
	}

	vk::PresentModeKHR GraphicsDevice::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, vk::PresentModeKHR preferredMode)
	{
		bool doWeHaveRelaxedFifo{false};
		for (const auto & availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == preferredMode) { return availablePresentMode; }
			if (availablePresentMode == vk::PresentModeKHR::eFifoRelaxed) { doWeHaveRelaxedFifo = true; }
		}

		// If we are unable to find the preferred mode, but we have relaxed fifo, we'll just use that.
		if (doWeHaveRelaxedFifo) { return vk::PresentModeKHR::eFifoRelaxed; }

		// If we can't find the preferred mode, and we don't have relaxed fifo, the standard ensures that we will always have fifo.
		return vk::PresentModeKHR::eFifo;
	}

} // namespace gen
