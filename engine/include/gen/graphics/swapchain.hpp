// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "device.hpp"
#include "gen/core.hpp"
#include "gen/core/monoInstance.hpp"
#include "gen/windowing/window.hpp"

// external
#include <vulkan/vulkan.hpp>

// std
#include <vector>

namespace gen
{
	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities{};
		std::vector<vk::SurfaceFormatKHR> availableFormats{};
		vk::SurfaceFormatKHR selectedFormat{};
		std::vector<vk::PresentModeKHR> availablePresentModes{};
		vk::PresentModeKHR selectedPresentMode{};
	};

	class Swapchain
	{
	public:
		Swapchain(const Window & window, const Device & device);
		~Swapchain();

		Swapchain(Swapchain const &)			 = delete;
		Swapchain(Swapchain &&)					 = delete;
		Swapchain & operator=(Swapchain const &) = delete;
		Swapchain & operator=(Swapchain &&)		 = delete;

		/// Getters
		GEN_NODISCARD vk::SwapchainKHR const & getSwapChain() const { return m_swapChain.get(); }
		GEN_NODISCARD vk::Extent2D const & getExtent() const { return m_swapChainInfo.imageExtent; }
		GEN_NODISCARD vk::Format const & getFormat() const { return m_swapChainInfo.imageFormat; }
		GEN_NODISCARD std::vector<vk::Image> const & getImages() const { return m_swapChainImages; }
		GEN_NODISCARD std::vector<vk::UniqueImageView> const & getImageViews() const { return m_swapChainImageViews; }

	private:
		void createSwapChain(const Window & window, const Device & device);
		void createImageViews(const Device & device);

		void recreateSwapChain(const Window & window, const Device & device);

		/// Helpers

		static SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface);
		static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> & availableFormats);
		static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, vk::PresentModeKHR preferredMode);

		SwapChainSupportDetails m_swapChainSupport{};
		vk::SwapchainCreateInfoKHR m_swapChainInfo{};
		vk::UniqueSwapchainKHR m_swapChain{};
		std::vector<vk::Image> m_swapChainImages{};
		std::vector<vk::UniqueImageView> m_swapChainImageViews{};

		Logger m_logger{"graphics"};
	};
} // namespace gen