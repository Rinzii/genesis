// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "core.hpp"
#include "windowing/window.hpp"

// external
#include <vulkan/vulkan.hpp>

// std
#include <vector>

namespace gen
{
	struct Gpu
	{
		vk::PhysicalDevice physicalDevice{};
		vk::PhysicalDeviceProperties properties{};
		u32 queueFamily{};
	};

	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities{};
		std::vector<vk::SurfaceFormatKHR> availableFormats{};
		vk::SurfaceFormatKHR selectedFormat{};
		std::vector<vk::PresentModeKHR> availablePresentModes{};
		vk::PresentModeKHR selectedPresentMode{};
	};

	class GraphicsDevice
	{
	public:
		GraphicsDevice(const GraphicsDevice &)			   = delete;
		GraphicsDevice & operator=(const GraphicsDevice &) = delete;
		GraphicsDevice(GraphicsDevice &&)				   = delete;
		GraphicsDevice & operator=(GraphicsDevice &&)	   = delete;

		explicit GraphicsDevice(const Window & window, std::string const & appName);
		~GraphicsDevice();

		/// Getters

		GEN_NODISCARD vk::Instance const & getInstance() const { return m_instance.get(); }
		GEN_NODISCARD vk::SurfaceKHR const & getSurface() const { return m_surface.get(); }
		GEN_NODISCARD vk::PhysicalDevice const & getPhysicalDevice() const { return m_gpu.physicalDevice; }
		GEN_NODISCARD vk::Device const & getDevice() const { return m_device.get(); }
		GEN_NODISCARD vk::UniqueDevice const & getUniqueDevice() const { return m_device; }
		GEN_NODISCARD vk::Queue const & getGraphicsQueue() const { return m_graphicsQueue; }
		GEN_NODISCARD vk::SwapchainKHR const & getSwapChain() const { return m_swapChain.get(); }
		GEN_NODISCARD vk::SwapchainCreateInfoKHR const & getSwapChainInfo() const { return m_swapChainInfo; }
		GEN_NODISCARD std::vector<vk::Image> const & getSwapChainImages() const { return m_swapChainImages; }
		GEN_NODISCARD std::vector<vk::UniqueImageView> const & getSwapChainImageViews() const { return m_swapChainImageViews; }

		/// Setters

	private:
		void createInstance(const std::string & appName, const std::string & engineName, const gen::u32 & apiVersion);
		void createSurface(const Window & window);
		void pickPhysicalDevice();
		void createLogicalDevice();
		void createSwapChain(const Window & window);
		void createImageViews();

		/// Helpers

		u32 findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface);
		static SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface);
		static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> & availableFormats);
		static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, vk::PresentModeKHR preferredMode);

		vk::UniqueInstance m_instance{};
		vk::UniqueSurfaceKHR m_surface{};
		Gpu m_gpu{};
		vk::UniqueDevice m_device{};
		vk::Queue m_graphicsQueue{};
		SwapChainSupportDetails m_swapChainSupport{};
		vk::SwapchainCreateInfoKHR m_swapChainInfo{};
		vk::UniqueSwapchainKHR m_swapChain{};
		std::vector<vk::Image> m_swapChainImages{};
		std::vector<vk::UniqueImageView> m_swapChainImageViews{};

		Logger m_logger{"graphics"};
	};
} // namespace gen
