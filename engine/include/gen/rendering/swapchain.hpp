// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>
#include <set>

namespace gen
{
	class Device;

	struct SwapchainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	struct SwapchainProperties
	{
		vk::SwapchainKHR oldSwapchain;
		vk::SwapchainCreateInfoKHR createInfo;
		u32 maxImageCount{2}; // Default double buffering
		vk::Extent2D extent;
		vk::SurfaceFormatKHR surfaceFormat;
		u32 imageArrayLayers;
		vk::ImageUsageFlags imageUsage;
		vk::SurfaceTransformFlagBitsKHR preTransform;
		vk::CompositeAlphaFlagBitsKHR compositeAlpha;
		vk::PresentModeKHR presentMode;
	};

	class Swapchain
	{
	public:
		Swapchain(Swapchain & oldSwapchain, u32 maxImageCount);

		Swapchain(Swapchain & oldSwapchain, const vk::Extent2D & extent);

		Swapchain(Swapchain & oldSwapchain, const vk::ImageUsageFlags & imageUsageFlags);

		Swapchain(Swapchain & oldSwapchain, const vk::Extent2D & extent, vk::SurfaceTransformFlagBitsKHR preTransform);

		explicit Swapchain(
			const vk::PresentModeKHR & desiredPresentMode,
			const std::vector<vk::PresentModeKHR> & presentModePriorityList =
				{vk::PresentModeKHR::eMailbox, vk::PresentModeKHR::eFifoRelaxed, vk::PresentModeKHR::eFifo},
			const std::vector<vk::SurfaceFormatKHR> & surfaceFormatPriorityList =
				{{vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}, {vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}},
			const vk::Extent2D & extent							 = {},
			const u32 & maxImageCount							 = 2,
			const vk::SurfaceTransformFlagBitsKHR & preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity,
			const vk::ImageUsageFlags & imageUsagePriorityList	 = vk::ImageUsageFlagBits::eColorAttachment);

		Swapchain(
			Swapchain & oldSwapchain,
			const vk::PresentModeKHR & desiredPresentMode,
			const std::vector<vk::PresentModeKHR> & presentModePriorityList =
				{vk::PresentModeKHR::eMailbox, vk::PresentModeKHR::eFifoRelaxed, vk::PresentModeKHR::eFifo},
			const std::vector<vk::SurfaceFormatKHR> & surfaceFormatPriorityList =
				{{vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}, {vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}},
			const vk::Extent2D & extent							 = {},
			const u32 & maxImageCount							 = 2,
			const vk::SurfaceTransformFlagBitsKHR & preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity,
			const vk::ImageUsageFlags & imageUsagePriorityList	 = vk::ImageUsageFlagBits::eColorAttachment);

		~Swapchain();

		Swapchain(const Swapchain &)			 = delete;
		Swapchain(Swapchain &&)					 = delete;
		Swapchain & operator=(const Swapchain &) = delete;
		Swapchain & operator=(Swapchain &&)		 = delete;

		std::pair<vk::Result, u32> acquireNextImage(const vk::Semaphore & semaphore, const vk::Fence fence = nullptr);

		// Getters

		[[nodiscard]] vk::SwapchainKHR getHandle() const { return m_handle.get(); }

		[[nodiscard]] const vk::Extent2D & getExtent() const { return m_properties.extent; }

		[[nodiscard]] vk::Format getFormat() const { return m_properties.surfaceFormat.format; }

		[[nodiscard]] const std::vector<vk::Image> & getImages() const { return m_images; }

		[[nodiscard]] vk::SurfaceTransformFlagBitsKHR getTransform() const { return m_properties.preTransform; }

		[[nodiscard]] vk::PresentModeKHR getPresentMode() const { return m_properties.presentMode; }

		[[nodiscard]] vk::ImageUsageFlags getUsageFlags() const { return m_properties.imageUsage; }

	private:
		vk::UniqueSwapchainKHR m_handle;

		std::vector<vk::Image> m_images;

		std::vector<vk::SurfaceFormatKHR> m_surfaceFormats;

		std::vector<vk::PresentModeKHR> m_presentModes;

		SwapchainSupportDetails m_supportDetails;

		SwapchainProperties m_properties;

		std::vector<vk::PresentModeKHR> m_presentModePriorityList;

		std::vector<vk::SurfaceFormatKHR> m_surfaceFormatPriorityList;

		std::set<vk::ImageUsageFlagBits> m_imageUsagePriorityList;
	};

} // namespace gen