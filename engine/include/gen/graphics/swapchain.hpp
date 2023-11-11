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

	struct SwapChainBuffer
	{
		vk::Image image{};
		vk::UniqueImageView view{};
	};

	class SwapChain
	{
	public:
		SwapChain();

		SwapChain(SwapChain const &)			 = delete;
		SwapChain(SwapChain &&)					 = delete;
		SwapChain & operator=(SwapChain const &) = delete;
		SwapChain & operator=(SwapChain &&)		 = delete;

		void create(u32 width, u32 height, bool vsync = false);

		vk::Result acquireNextImage(vk::Semaphore presentCompleteSemaphore, u32 * imageIndex, u64 timeout = std::numeric_limits<u64>::max());

		vk::Result queuePresent(vk::Queue queue, u32 imageIndex, vk::Semaphore waitSemaphore = nullptr);

		/// Getters

		[[nodiscard]] vk::SwapchainKHR getSwapChain() const { return m_swapChain.get(); }
		[[nodiscard]] const std::vector<vk::Image> & getImages() const { return m_images; }
		[[nodiscard]] const SwapChainBuffer & getBufferAt(std::size_t i) const;
		[[nodiscard]] u32 getImageCount() const { return m_imageCount; }
		[[nodiscard]] const SwapChainSupportDetails & getSupportDetails() const { return m_supportDetails; }

	private:
		/// Helpers

		static SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool vsync);
		static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> & availableFormats);
		static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, bool vsync);
		static vk::Extent2D chooseSwapExtent(const SwapChainSupportDetails & swapChainSupportDetails, u32 & width, u32 & height);

		vk::UniqueSwapchainKHR m_swapChain{};
		SwapChainSupportDetails m_supportDetails{};
		std::vector<vk::Image> m_images{};
		std::vector<SwapChainBuffer> m_buffers{};
		u32 m_imageCount{0};

	private:
		Logger m_logger{"graphics"};
	};
} // namespace gen