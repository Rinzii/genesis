// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once


#include "core.hpp"
#include "windowing/window.hpp"
#include "util/nonCopyable.hpp"
#include "util/nonMovable.hpp"

#include <vulkan/vulkan.hpp>



namespace gen
{
	class GraphicsDevice : public NonCopyable, public NonMovable
	{
	public:
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



	private:
		void createInstance(std::string const & appName,
							std::string const & engineName,
							std::vector<std::string> const & layers,
							std::vector<std::string> const & extensions,
							u32 const & apiVersion
		);

		void createDebugMessenger();






		vk::Instance m_instance;
		vk::DebugUtilsMessengerEXT m_debugMessenger;
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		vk::SurfaceKHR m_surface;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;
		vk::CommandPool m_commandPool;

		Window &m_window;

		const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
		const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};


	};
}


