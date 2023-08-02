// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once


#include "core.hpp"
#include "windowing/window.hpp"

#include <vulkan/vulkan.hpp>

#include <memory>



namespace gen
{

	struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

	struct QueueFamilyIndices
    {
        std::optional<u32> graphicsFamily;
        std::optional<u32> presentFamily;

        GEN_NODISCARD bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

	class GraphicsDevice
	{
	public:
		GraphicsDevice(const GraphicsDevice&) = delete;
		GraphicsDevice& operator=(const GraphicsDevice&) = delete;
		GraphicsDevice(GraphicsDevice&&) = delete;
		GraphicsDevice& operator=(GraphicsDevice&&) = delete;


#if defined(NDEBUG) || defined(GEN_NDEBUG)
		static constexpr bool enableValidationLayers = false;
#else
		static constexpr bool enableValidationLayers = true;
#endif

		explicit GraphicsDevice(Window &window);
		~GraphicsDevice();

		GEN_NODISCARD vk::CommandPool getCommandPool() const { return m_commandPool; }
		GEN_NODISCARD vk::Device getDevice() const { return m_device; }
		GEN_NODISCARD vk::SurfaceKHR getSurface() const { return m_surface; }
		GEN_NODISCARD vk::Queue getGraphicsQueue() const { return m_graphicsQueue; }
		GEN_NODISCARD vk::Queue getPresentQueue() const { return m_presentQueue; }

		GEN_NODISCARD SwapChainSupportDetails getSwapChainSupport()
		{
			return querySwapChainSupport(m_physicalDevice);
		}

		u32 findMemoryType(u32 typeFilter, vk::MemoryPropertyFlags properties);

		QueueFamilyIndices findPhysicalQueueFamilies()
		{
			return findQueueFamilies(m_physicalDevice);
		};

		vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);

		// Buffer helper functions

		void createBuffer(vk::DeviceSize size,
						  vk::BufferUsageFlags usage,
						  vk::MemoryPropertyFlags properties,
						  vk::Buffer& buffer,
						  vk::DeviceMemory& bufferMemory
		);
		vk::CommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(vk::CommandBuffer commandBuffer);
		void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);
		void copyBufferToImage(vk::Buffer buffer, vk::Image image, u32 width, u32 height, u32 layerCount);
		void createImageWithInfo(const vk::ImageCreateInfo & imageInfo,
								 vk::MemoryPropertyFlags properties,
								 vk::Image & image,
								 vk::DeviceMemory & imageMemory
        );


	private:
		void createInstance(std::string const & appName,
							std::string const & engineName,
							std::vector<std::string> const & layers,
							std::vector<std::string> const & extensions,
							u32 const & apiVersion
		);
		void createDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createCommandPool();

		bool isDeviceSuitable(vk::PhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);
		bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device);



		vk::Instance m_instance;
		vk::DebugUtilsMessengerEXT m_debugMessenger;
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		vk::SurfaceKHR m_surface;

		vk::PhysicalDeviceProperties m_physicalDeviceProperties;

		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;
		vk::CommandPool m_commandPool;

		Window &m_window; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

		const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
		const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};


	};
}


