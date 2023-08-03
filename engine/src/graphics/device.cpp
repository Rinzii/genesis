// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/device.hpp"
#include "graphics/vkHelpers.hpp"
#include "logger/log.hpp"

#include <GLFW/glfw3.h>

namespace gen
{

	GraphicsDevice::GraphicsDevice(gen::Window & window) : m_window{window}
	{
		// TODO: Allow for the vulkan api to be specified in a config file or through command line arguments.
		// We could also dynamically check for the highest supported version of the vulkan api, but that feels outside the scope of this project.
		createInstance(window.getTitle(), "Genesis Engine", validationLayers, {}, VK_API_VERSION_1_0);

	    //createDebugMessenger();
        //createSurface();
		//pickPhysicalDevice();
		//createLogicalDevice();
		//createCommandPool();
	}

	GraphicsDevice::~GraphicsDevice()
	{
		m_device.destroyCommandPool(m_commandPool);
        m_device.destroy();

		//NOLINTNEXTLINE validation layers are enabled conditionally and this trips up clang-tidy
		if (enableValidationLayers) { m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger); }

		m_instance.destroySurfaceKHR(m_surface);
		m_instance.destroy();
	}

	void GraphicsDevice::createInstance(const std::string & appName, const std::string & engineName, const std::vector<std::string> & layers,
											 const std::vector<std::string> & extensions, const gen::u32 & apiVersion)
	{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const vkdl;
		auto vkGetInstanceProcAddr = vkdl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		if (enableValidationLayers) //NOLINT validation layers are enabled conditionally and this trips up clang-tidy
        {
			if (!vk::util::checkValidationLayerSupport(validationLayers))
			{
				gen::logger::error("vulkan", "validation layers requested, but not available!");
				throw std::runtime_error("validation layers requested, but not available!");
			}
        }

		// TODO: Allow for the application to be able to specify its version itself. Instead of just using the engine version.
		vk::ApplicationInfo const appInfo(
			appName.c_str(),
			gen::version_v.getVersion(),
			engineName.c_str(),
			gen::version_v.getVersion(),
			apiVersion);

		vk::InstanceCreateInfo const createInfo({}, &appInfo);

		auto enabledLayers = vk::util::gatherLayers(layers
#ifndef GEN_NDEBUG
													,
													vk::enumerateInstanceLayerProperties()
#endif
		);

		auto enabledExtensions = vk::util::gatherExtensions(extensions
#ifndef GEN_NDEBUG
															,
															vk::enumerateInstanceExtensionProperties()
#endif
		);

		m_instance = vk::createInstance(vk::util::makeInstanceCreateInfoChain(appInfo, enabledLayers, enabledExtensions).get<vk::InstanceCreateInfo>());

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance);
#endif
	}

	/*
	QueueFamilyIndices GraphicsDevice::findQueueFamilies(vk::PhysicalDevice device)
	{
		QueueFamilyIndices indices;

		std::vector<vk::QueueFamilyProperties> const queueFamilies = device.getQueueFamilyProperties();

		gen::u32 index = 0;
		for (const auto & queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphicsFamily = index;
			}

			vk::Bool32 const presentSupport = device.getSurfaceSupportKHR(index, m_surface);
			if (presentSupport != 0U)
			{
				indices.presentFamily = index;
			}

			if (indices.isComplete())
			{
				break;
			}

			index++;
		}

		return indices;
	}

	bool GraphicsDevice::checkDeviceExtensionSupport(vk::PhysicalDevice device)
	{
		std::vector<vk::ExtensionProperties> const availableExtensions = device.enumerateDeviceExtensionProperties();

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto & extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName.data());
		}

		return requiredExtensions.empty();
	}


	void GraphicsDevice::createDebugMessenger()
	{
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
		m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(vk::util::makeDebugUtilsMessengerCreateInfoEXT());
#endif
	}

	bool GraphicsDevice::isDeviceSuitable(const vk::PhysicalDevice & device)
	{
		QueueFamilyIndices const indices = findQueueFamilies(device);

		bool const extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails const swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}
	*/
	/*

	void GraphicsDevice::createSurface()
    {
        m_surface = m_window.createWindowSurface(m_instance);
    }

	void GraphicsDevice::pickPhysicalDevice()
	{
		auto physicalDevices = m_instance.enumeratePhysicalDevices();
        if (physicalDevices.empty())
        {
			gen::logger::error("vulkan", "Failed to find GPUs with Vulkan support!");
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        }

        for (const auto & device : physicalDevices)
        {
            if (isDeviceSuitable(device))
            {
				gen::logger::debug("vulkan", std::format("Found suitable GPU: {}", device.getProperties().deviceName.data()));
                m_physicalDevice = device;
                break;
            }
        }

        if (!m_physicalDevice)
        {
			gen::logger::error("vulkan", "Failed to find a suitable GPU!");
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
	}

*/

	/*
	void GraphicsDevice::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::set<gen::u32> const uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0F;
        for (gen::u32 const queueFamily : uniqueQueueFamilies)
        {
            queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags(), queueFamily, 1, &queuePriority);
        }

        vk::PhysicalDeviceFeatures const deviceFeatures;

        vk::DeviceCreateInfo const createInfo(vk::DeviceCreateFlags(), queueCreateInfos, {}, {}, &deviceFeatures);

        m_device = m_physicalDevice.createDevice(createInfo);

        m_graphicsQueue = m_device.getQueue(indices.graphicsFamily.value(), 0);
        m_presentQueue = m_device.getQueue(indices.presentFamily.value(), 0);
	}

    void GraphicsDevice::createCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_physicalDevice);

        vk::CommandPoolCreateInfo const poolInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
												 queueFamilyIndices.graphicsFamily.value()
		);

        m_commandPool = m_device.createCommandPool(poolInfo);
    }





	SwapChainSupportDetails GraphicsDevice::querySwapChainSupport(vk::PhysicalDevice device)
	{
		SwapChainSupportDetails details;

        details.capabilities = device.getSurfaceCapabilitiesKHR(m_surface);
        details.formats = device.getSurfaceFormatsKHR(m_surface);
        details.presentModes = device.getSurfacePresentModesKHR(m_surface);

        return details;
	}

	void GraphicsDevice::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer & buffer,
									  vk::DeviceMemory & bufferMemory)
	{
		vk::BufferCreateInfo const bufferInfo({}, size, usage, vk::SharingMode::eExclusive);

        buffer = m_device.createBuffer(bufferInfo);

        vk::MemoryRequirements const memRequirements = m_device.getBufferMemoryRequirements(buffer);

        vk::MemoryAllocateInfo const allocInfo(memRequirements.size, findMemoryType(memRequirements.memoryTypeBits, properties));

        bufferMemory = m_device.allocateMemory(allocInfo);

        m_device.bindBufferMemory(buffer, bufferMemory, 0);
	}

	vk::CommandBuffer GraphicsDevice::beginSingleTimeCommands()
    {
        vk::CommandBufferAllocateInfo const allocInfo(m_commandPool, vk::CommandBufferLevel::ePrimary, 1);

        vk::CommandBuffer commandBuffer = m_device.allocateCommandBuffers(allocInfo)[0];

        vk::CommandBufferBeginInfo const beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        commandBuffer.begin(beginInfo);

        return commandBuffer;
    }

	void GraphicsDevice::endSingleTimeCommands(vk::CommandBuffer commandBuffer)
    {
        commandBuffer.end();

        vk::SubmitInfo const submitInfo(0, nullptr, nullptr, 1, &commandBuffer, 0, nullptr);

        m_graphicsQueue.submit(submitInfo, nullptr);
        m_graphicsQueue.waitIdle();

        m_device.freeCommandBuffers(m_commandPool, commandBuffer);
    }

	void GraphicsDevice::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
    {
        vk::CommandBuffer const commandBuffer = beginSingleTimeCommands();

        vk::BufferCopy const copyRegion(0, 0, size);
        commandBuffer.copyBuffer(srcBuffer, dstBuffer, copyRegion);

        endSingleTimeCommands(commandBuffer);
    }
	void GraphicsDevice::copyBufferToImage(vk::Buffer buffer, vk::Image image, u32 width, u32 height, u32 layerCount)
	{
		vk::CommandBuffer const commandBuffer = beginSingleTimeCommands();

        vk::BufferImageCopy const region(0, 0, 0, vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, layerCount),
                                         vk::Offset3D(0, 0, 0), vk::Extent3D(width, height, 1)
        );

        commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);

        endSingleTimeCommands(commandBuffer);
	}
	void GraphicsDevice::createImageWithInfo(const vk::ImageCreateInfo & imageInfo,
											 vk::MemoryPropertyFlags properties,
											 vk::Image & image,
											 vk::DeviceMemory & imageMemory)
	{
		image = m_device.createImage(imageInfo);

        vk::MemoryRequirements const memRequirements = m_device.getImageMemoryRequirements(image);

        vk::MemoryAllocateInfo const allocInfo(memRequirements.size, findMemoryType(memRequirements.memoryTypeBits, properties));

        imageMemory = m_device.allocateMemory(allocInfo);

        m_device.bindImageMemory(image, imageMemory, 0);
	}
	vk::Format GraphicsDevice::findSupportedFormat(const std::vector<vk::Format> & candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
	{
		for (vk::Format const format : candidates)
        {
            vk::FormatProperties const props = m_physicalDevice.getFormatProperties(format);

            if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }

            if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

		gen::logger::error("vulkan", "failed to find supported format!");
        throw std::runtime_error("failed to find supported format!");
	}
	u32 GraphicsDevice::findMemoryType(u32 typeFilter, vk::MemoryPropertyFlags properties)
	{
		vk::PhysicalDeviceMemoryProperties const memProperties = m_physicalDevice.getMemoryProperties();

        for (u32 i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if (((typeFilter & (1 << i)) != 0U) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

		gen::logger::error("vulkan", "failed to find suitable memory type!");
        throw std::runtime_error("failed to find suitable memory type!");
	}
*/
}
