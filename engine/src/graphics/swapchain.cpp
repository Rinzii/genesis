// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/swapchain.hpp"
#include "gen/graphics/graphicsExceptions.hpp"

#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include "gen/graphics/vkHelpers.hpp"

#include <algorithm>

namespace gen
{

	SwapChain::SwapChain(
		vk::SurfaceKHR surface,
		const vk::Extent2D & extent,
		const u32 & imageCount,
		const vk::PresentModeKHR & presentMode,
		const vk::ImageUsageFlags & imageUsage)
		: SwapChain(*this, surface, extent, imageCount, presentMode, imageUsage)
	{
	}

	SwapChain::SwapChain(
		SwapChain & oldSwapChain,
		vk::SurfaceKHR surface,
		const vk::Extent2D & extent,
		const u32 & imageCount,
		const vk::PresentModeKHR & presentMode,
		const vk::ImageUsageFlags & imageUsage)
	{
		m_supportDetails = querySwapChainSupport(Device::self(), surface);

		std::vector<vk::SurfaceFormatKHR> const requestedSurfaceFormats = {
			{vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}, {vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}};

		auto const selectedSurfaceFormat = selectSurfaceFormat(m_supportDetails.formats, requestedSurfaceFormats);

		vk::SwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.flags					   = vk::SwapchainCreateFlagsKHR();
		swapchainCreateInfo.surface					   = surface;
		swapchainCreateInfo.minImageCount =
			selectImageCount(imageCount, m_supportDetails.capabilities.minImageCount, m_supportDetails.capabilities.maxImageCount);
		swapchainCreateInfo.imageFormat		= selectedSurfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = selectedSurfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent		= selectExtent(
			extent, m_supportDetails.capabilities.minImageExtent, m_supportDetails.capabilities.maxImageExtent, m_supportDetails.capabilities.currentExtent);
		swapchainCreateInfo.imageArrayLayers = selectImageArrayLayers(1, m_supportDetails.capabilities.maxImageArrayLayers);
		swapchainCreateInfo.imageUsage		 = selectImageUsage(imageUsage, m_supportDetails.capabilities.supportedUsageFlags);
		swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
		swapchainCreateInfo.preTransform	 = selectPreTransform(
			m_supportDetails.capabilities.currentTransform, m_supportDetails.capabilities.supportedTransforms, m_supportDetails.capabilities.currentTransform);
		swapchainCreateInfo.compositeAlpha =
			selectCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque, m_supportDetails.capabilities.supportedCompositeAlpha);
		swapchainCreateInfo.presentMode	 = selectPresentMode(m_supportDetails.presentModes, presentMode);
		swapchainCreateInfo.oldSwapchain = oldSwapChain.m_handle.get();

		m_createInfo = swapchainCreateInfo;

		m_handle = Device::self().getDevice().createSwapchainKHRUnique(m_createInfo);

		if (m_handle.get() == nullptr) { throw gen::VulkanException("Failed to create swapchain!"); }

		m_images = Device::self().getDevice().getSwapchainImagesKHR(m_handle.get());
	}

	SwapChain::SwapChain(SwapChain & oldSwapChain, const u32 & imageCount)
		: SwapChain(
			  *this,
			  oldSwapChain.m_createInfo.surface,
			  oldSwapChain.m_createInfo.imageExtent,
			  imageCount,
			  oldSwapChain.m_createInfo.presentMode,
			  oldSwapChain.m_createInfo.imageUsage)
	{
	}

	SwapChain::SwapChain(SwapChain & oldSwapChain, const vk::Extent2D & extent)
		: SwapChain(
			  *this,
			  oldSwapChain.m_createInfo.surface,
			  extent,
			  oldSwapChain.m_createInfo.minImageCount,
			  oldSwapChain.m_createInfo.presentMode,
			  oldSwapChain.m_createInfo.imageUsage)
	{
	}

	vk::Result SwapChain::acquireNextImage(vk::Semaphore presentCompleteSemaphore, vk::Fence fence, u32 * imageIndex, u64 timeout)
	{
		return Device::self().getDevice().acquireNextImageKHR(m_handle.get(), timeout, presentCompleteSemaphore, fence, imageIndex);
	}

	SwapChainSupportDetails SwapChain::querySwapChainSupport(Device & device, vk::SurfaceKHR surface)
	{
		SwapChainSupportDetails details;

		details.capabilities = device.getGpu().physicalDevice.getSurfaceCapabilitiesKHR(surface);
		details.formats		 = device.getGpu().physicalDevice.getSurfaceFormatsKHR(surface);
		details.presentModes = device.getGpu().physicalDevice.getSurfacePresentModesKHR(surface);

		return details;
	}

	u32 SwapChain::selectImageCount(u32 requestedImageCount, u32 minImageCount, u32 maxImageCount)
	{
		requestedImageCount = std::min(requestedImageCount, maxImageCount);
		requestedImageCount = std::max(requestedImageCount, minImageCount);

		return requestedImageCount;
	}

	vk::Extent2D SwapChain::selectExtent(vk::Extent2D requestedExtent, vk::Extent2D minExtent, vk::Extent2D maxExtent, vk::Extent2D currentExtent)
	{
		if (requestedExtent.width < 1 || requestedExtent.height < 1) { return currentExtent; }

		requestedExtent.width = std::max(requestedExtent.width, minExtent.width);
		requestedExtent.width = std::min(requestedExtent.width, maxExtent.width);

		requestedExtent.height = std::max(requestedExtent.height, minExtent.height);
		requestedExtent.height = std::min(requestedExtent.height, maxExtent.height);

		return requestedExtent;
	}

	vk::SurfaceFormatKHR SwapChain::selectSurfaceFormat(
		const std::vector<vk::SurfaceFormatKHR> & surfaceFormats, const std::vector<vk::SurfaceFormatKHR> & requestedFormats)
	{
		std::vector<vk::SurfaceFormatKHR>::const_iterator surfaceFormatIterator;

		for (const auto & requestedFormat : requestedFormats)
		{
			surfaceFormatIterator = std::find_if(
				surfaceFormats.begin(),
				surfaceFormats.end(),
				[requestedFormat](const vk::SurfaceFormatKHR & surface)
				{
					if (surface.format == requestedFormat.format && surface.colorSpace == requestedFormat.colorSpace) { return true; }
					return false;
				});

			if (surfaceFormatIterator != surfaceFormats.end()) { break; }
		}

		if (surfaceFormatIterator == surfaceFormats.end()) { surfaceFormatIterator = surfaceFormats.begin(); }

		return *surfaceFormatIterator;
	}

	u32 SwapChain::selectImageArrayLayers(u32 requestedImageArrayLayers, u32 maxImageArrayLayers)
	{
		requestedImageArrayLayers = std::min(requestedImageArrayLayers, maxImageArrayLayers);
		requestedImageArrayLayers = std::max(requestedImageArrayLayers, 1U);

		return requestedImageArrayLayers;
	}

	vk::ImageUsageFlags SwapChain::selectImageUsage(vk::ImageUsageFlags requestedImageUsage, vk::ImageUsageFlags supportedImageUsage)
	{
		if (requestedImageUsage & supportedImageUsage) { return requestedImageUsage; }

		static const std::vector<vk::ImageUsageFlagBits> imgUsageFlags = {
			vk::ImageUsageFlagBits::eColorAttachment,
			vk::ImageUsageFlagBits::eStorage,
			vk::ImageUsageFlagBits::eSampled,
			vk::ImageUsageFlagBits::eTransferDst,
		};

		for (const auto & imgUsageFlag : imgUsageFlags)
		{
			if (requestedImageUsage & imgUsageFlag) { return imgUsageFlag; }
		}

		throw gen::VulkanException("Failed to find suitable image usage!");
	}

	vk::SurfaceTransformFlagBitsKHR SwapChain::selectPreTransform(
		vk::SurfaceTransformFlagBitsKHR requestedPreTransform,
		vk::SurfaceTransformFlagsKHR supportedPreTransform,
		vk::SurfaceTransformFlagBitsKHR currentPreTransform)
	{
		if (supportedPreTransform & requestedPreTransform) { return requestedPreTransform; }

		return currentPreTransform;
	}

	vk::CompositeAlphaFlagBitsKHR SwapChain::selectCompositeAlpha(
		vk::CompositeAlphaFlagBitsKHR requestedCompositeAlpha, vk::CompositeAlphaFlagsKHR supportedCompositeAlpha)
	{
		if (supportedCompositeAlpha & requestedCompositeAlpha) { return requestedCompositeAlpha; }

		static const std::vector<vk::CompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
			vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
			vk::CompositeAlphaFlagBitsKHR::eInherit,
		};

		for (const auto & compositeAlphaFlag : compositeAlphaFlags)
		{
			if (compositeAlphaFlag & supportedCompositeAlpha) { return compositeAlphaFlag; }
		}

		throw gen::VulkanException("Failed to find suitable composite alpha!");
	}

	vk::PresentModeKHR SwapChain::selectPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, vk::PresentModeKHR requestedPresentMode)
	{
		bool doWeHaveRelaxedFifo{false};
		for (const auto & availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == requestedPresentMode) { return availablePresentMode; }

			if (availablePresentMode == vk::PresentModeKHR::eFifoRelaxed) { doWeHaveRelaxedFifo = true; }
		}

		if (doWeHaveRelaxedFifo) { return vk::PresentModeKHR::eFifoRelaxed; }

		return vk::PresentModeKHR::eFifo;
	}

} // namespace gen