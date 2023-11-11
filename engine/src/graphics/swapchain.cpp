// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/swapchain.hpp"
#include "gen/graphics/graphicsExceptions.hpp"

#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include "gen/graphics/vkHelpers.hpp"

namespace gen
{

	constexpr auto desiredSrgbFormats_v = std::array{vk::Format::eB8G8R8A8Srgb, vk::Format::eR8G8B8A8Srgb};

	SwapChain::SwapChain()
	{
	}

	void SwapChain::create(u32 width, u32 height, bool vsync)
	{
		vk::SwapchainKHR oldSwapChain = m_swapChain.get();

		m_supportDetails = querySwapChainSupport(Device::self().getGpu().physicalDevice, Device::self().getSurface(), vsync);
		assert(!m_supportDetails.availableFormats.empty());

		m_supportDetails.selectedFormat = chooseSwapSurfaceFormat(m_supportDetails.availableFormats);

		vk::Extent2D const swapchainExtent = chooseSwapExtent(m_supportDetails, width, height);

		// determine the number of images
		u32 desiredNumberOfSwapchainImages = m_supportDetails.capabilities.minImageCount + 1;
		if ((m_supportDetails.capabilities.maxImageCount > 0) && (desiredNumberOfSwapchainImages > m_supportDetails.capabilities.maxImageCount))
		{
			desiredNumberOfSwapchainImages = m_supportDetails.capabilities.maxImageCount;
		}

		vk::SwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.flags					   = vk::SwapchainCreateFlagsKHR();
		swapchainCreateInfo.surface					   = Device::self().getSurface();
		swapchainCreateInfo.minImageCount			   = desiredNumberOfSwapchainImages;
		swapchainCreateInfo.imageFormat				   = m_supportDetails.selectedFormat.format;
		swapchainCreateInfo.imageColorSpace			   = m_supportDetails.selectedFormat.colorSpace;
		swapchainCreateInfo.imageExtent				   = swapchainExtent;
		swapchainCreateInfo.imageArrayLayers		   = 1;
		swapchainCreateInfo.imageUsage				   = vk::ImageUsageFlagBits::eColorAttachment;
		swapchainCreateInfo.imageSharingMode		   = vk::SharingMode::eExclusive;
		swapchainCreateInfo.preTransform			   = vk::SurfaceTransformFlagBitsKHR::eIdentity;
		swapchainCreateInfo.compositeAlpha			   = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		swapchainCreateInfo.presentMode				   = m_supportDetails.selectedPresentMode;
		swapchainCreateInfo.clipped					   = vk::False;

		if (oldSwapChain) { swapchainCreateInfo.oldSwapchain = oldSwapChain; }

		m_swapChain = Device::self().getDevice().createSwapchainKHRUnique(swapchainCreateInfo);

		vk::utils::checkResult(Device::self().getDevice().getSwapchainImagesKHR(m_swapChain.get(), &m_imageCount, nullptr));

		m_images.resize(m_imageCount);
		vk::utils::checkResult(Device::self().getDevice().getSwapchainImagesKHR(m_swapChain.get(), &m_imageCount, m_images.data()));

		// Now we get the swapchain images and create the image views.
		m_buffers.resize(m_imageCount);
		for (u32 i = 0; i < m_imageCount; i++)
		{
			vk::ImageViewCreateInfo colorAttachmentView{};
			colorAttachmentView.format	   = m_supportDetails.selectedFormat.format;
			colorAttachmentView.components = {vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA};
			colorAttachmentView.subresourceRange.aspectMask		= vk::ImageAspectFlagBits::eColor;
			colorAttachmentView.subresourceRange.baseMipLevel	= 0;
			colorAttachmentView.subresourceRange.levelCount		= 1;
			colorAttachmentView.subresourceRange.baseArrayLayer = 0;
			colorAttachmentView.subresourceRange.layerCount		= 1;
			colorAttachmentView.viewType						= vk::ImageViewType::e2D;
			colorAttachmentView.flags							= {};

			m_buffers[i].image = m_images[i];

			colorAttachmentView.image = m_buffers[i].image;

			m_buffers[i].view = Device::self().getDevice().createImageViewUnique(colorAttachmentView);
		}
	}

	vk::Result SwapChain::acquireNextImage(vk::Semaphore presentCompleteSemaphore, u32 * imageIndex, u64 timeout)
	{
		return Device::self().getDevice().acquireNextImageKHR(m_swapChain.get(), timeout, presentCompleteSemaphore, nullptr, imageIndex);
	}

	vk::Result SwapChain::queuePresent(vk::Queue queue, u32 imageIndex, vk::Semaphore waitSemaphore)
	{
		vk::PresentInfoKHR presentInfo{};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains	   = &m_swapChain.get();
		presentInfo.pImageIndices  = &imageIndex;
		if (waitSemaphore != nullptr)
		{
			presentInfo.pWaitSemaphores	   = &waitSemaphore;
			presentInfo.waitSemaphoreCount = 1;
		}

		return queue.presentKHR(presentInfo);
	}

	SwapChainSupportDetails SwapChain::querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool vsync)
	{
		SwapChainSupportDetails details;
		details.capabilities		  = device.getSurfaceCapabilitiesKHR(surface);
		details.availableFormats	  = device.getSurfaceFormatsKHR(surface);
		details.availablePresentModes = device.getSurfacePresentModesKHR(surface);
		details.selectedPresentMode	  = chooseSwapPresentMode(details.availablePresentModes, vsync);
		return details;
	}

	vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> & availableFormats)
	{
		for (const auto & availableFormat : availableFormats)
		{
			if (availableFormat.colorSpace != vk::ColorSpaceKHR::eSrgbNonlinear) { continue; }

			if (std::ranges::find(desiredSrgbFormats_v, availableFormat.format) != desiredSrgbFormats_v.end()) { return availableFormat; }
		}

		// Currently we only support sRGB formats, so if we can't find one, we'll just throw an error.
		throw gen::VulkanException("Failed to find suitable surface format that supports SRGB!");
	}

	vk::PresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, bool vsync)
	{

		bool doWeHaveRelaxedFifo{false};
		bool doWeHaveImmediate{false};
		for (const auto & availablePresentMode : availablePresentModes)
		{
			// If we have vsync disabled, we'll try to find the mailbox mode.
			if (!vsync && availablePresentMode == vk::PresentModeKHR::eMailbox) { return availablePresentMode; }

			// If we have vsync disabled, we'll try to find the immediate mode if we are unable to find the mailbox mode.
			if (!vsync && availablePresentMode == vk::PresentModeKHR::eImmediate) { doWeHaveImmediate = true; }

			// Ideally if we have vsync enabled or are unable to find any of the above modes, we'll try to use relaxed fifo if possible.
			if (availablePresentMode == vk::PresentModeKHR::eFifoRelaxed) { doWeHaveRelaxedFifo = true; }
		}

		// If vsync was not requested, and we are unable to find Mailbox, but we have immediate, we'll just use that.
		if (doWeHaveImmediate) { return vk::PresentModeKHR::eImmediate; }

		// If we are unable to find the preferred mode, but we have relaxed fifo, we'll just use that.
		if (doWeHaveRelaxedFifo) { return vk::PresentModeKHR::eFifoRelaxed; }

		// If we can't find the preferred mode, and we don't have relaxed fifo, the standard ensures that we will always have fifo.
		return vk::PresentModeKHR::eFifo;
	}

	vk::Extent2D SwapChain::chooseSwapExtent(const SwapChainSupportDetails & swapChainSupportDetails, u32 & width, u32 & height)
	{
		vk::Extent2D swapchainExtent;

		if (swapChainSupportDetails.capabilities.currentExtent.width == std::numeric_limits<u32>::max())
		{
			// If the surface size is undefined, the size is set to the size of the images requested.
			swapchainExtent.width = std::clamp(
				static_cast<u32>(width), swapChainSupportDetails.capabilities.minImageExtent.width, swapChainSupportDetails.capabilities.maxImageExtent.width);
			swapchainExtent.height = std::clamp(
				static_cast<u32>(height),
				swapChainSupportDetails.capabilities.minImageExtent.height,
				swapChainSupportDetails.capabilities.maxImageExtent.height);
		}
		else
		{
			// If the surface size is defined, the swap chain size must match
			swapchainExtent = swapChainSupportDetails.capabilities.currentExtent;
			width			= swapchainExtent.width;
			height			= swapchainExtent.height;
		}

		return swapchainExtent;
	}
	const SwapChainBuffer & SwapChain::getBufferAt(std::size_t i) const
	{
		return m_buffers[i];
	}

} // namespace gen