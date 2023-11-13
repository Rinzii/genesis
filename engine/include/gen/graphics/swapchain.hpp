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
		std::vector<vk::SurfaceFormatKHR> formats{};
		std::vector<vk::PresentModeKHR> presentModes{};
	};

	class SwapChain
	{
	public:
		SwapChain(
			vk::SurfaceKHR surface,
			const vk::Extent2D & extent			   = {},
			const u32 & imageCount				   = 2,
			const vk::PresentModeKHR & presentMode = vk::PresentModeKHR::eFifo,
			const vk::ImageUsageFlags & imageUsage = vk::ImageUsageFlagBits::eColorAttachment);

		SwapChain(
			SwapChain & oldSwapChain,
			vk::SurfaceKHR surface,
			const vk::Extent2D & extent			   = {},
			const u32 & imageCount				   = 2,
			const vk::PresentModeKHR & presentMode = vk::PresentModeKHR::eFifo,
			const vk::ImageUsageFlags & imageUsage = vk::ImageUsageFlagBits::eColorAttachment);

		SwapChain(SwapChain & oldSwapChain, const u32 & imageCount);

		SwapChain(SwapChain & oldSwapChain, const vk::Extent2D & extent);

		SwapChain(SwapChain const &)			 = delete;
		SwapChain(SwapChain &&)					 = delete;
		SwapChain & operator=(SwapChain const &) = delete;
		SwapChain & operator=(SwapChain &&)		 = delete;

		vk::Result acquireNextImage(vk::Semaphore presentCompleteSemaphore, vk::Fence fence, u32 * imageIndex, u64 timeout = std::numeric_limits<u64>::max());

		/// Getters

		vk::Extent2D getExtent() const { return m_extent; }

	private:
		/// Helpers

		static SwapChainSupportDetails querySwapChainSupport(Device & device, vk::SurfaceKHR surface);

		u32 selectImageCount(u32 requestedImageCount, u32 minImageCount, u32 maxImageCount);

		vk::SurfaceFormatKHR selectSurfaceFormat(
			const std::vector<vk::SurfaceFormatKHR> & surfaceFormats, const std::vector<vk::SurfaceFormatKHR> & requestedFormats);

		vk::Extent2D selectExtent(vk::Extent2D requestedExtent, vk::Extent2D minExtent, vk::Extent2D maxExtent, vk::Extent2D currentExtent);

		u32 selectImageArrayLayers(u32 requestedImageArrayLayers, u32 maxImageArrayLayers);

		vk::ImageUsageFlags selectImageUsage(vk::ImageUsageFlags requestedImageUsage, vk::ImageUsageFlags supportedImageUsage);

		vk::SurfaceTransformFlagBitsKHR selectPreTransform(
			vk::SurfaceTransformFlagBitsKHR requestedPreTransform,
			vk::SurfaceTransformFlagsKHR supportedPreTransform,
			vk::SurfaceTransformFlagBitsKHR currentPreTransform);

		vk::CompositeAlphaFlagBitsKHR selectCompositeAlpha(
			vk::CompositeAlphaFlagBitsKHR requestedCompositeAlpha, vk::CompositeAlphaFlagsKHR supportedCompositeAlpha);

		vk::PresentModeKHR selectPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, vk::PresentModeKHR requestedPresentMode);

		SwapChainSupportDetails m_supportDetails{};

		vk::UniqueSwapchainKHR m_handle{};
		vk::SwapchainCreateInfoKHR m_createInfo{};
		std::vector<vk::Image> m_images{};
		vk::Extent2D m_extent{};

	private:
		Logger m_logger{"graphics"};
	};
} // namespace gen