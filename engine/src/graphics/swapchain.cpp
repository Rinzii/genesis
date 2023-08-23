// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/swapchain.hpp"
#include "gen/graphics/graphicsExceptions.hpp"

#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

namespace gen
{
	constexpr auto desiredSrgbFormats_v				  = std::array{vk::Format::eB8G8R8A8Srgb, vk::Format::eR8G8B8A8Srgb};
	constexpr vk::PresentModeKHR desiredPresentMode_v = {vk::PresentModeKHR::eMailbox};

	Swapchain::Swapchain(const Window & window, const Device & device, const vk::SurfaceKHR & surface)
	{
		createSwapChain(window, device, surface);
		createImageViews(device);
		m_logger.info("Swapchain created");
	}

	Swapchain::~Swapchain()
	{
		m_logger.info("Swapchain destructed");
	}

	void Swapchain::createSwapChain(const Window & window, const Device & device, const vk::SurfaceKHR & surface)
	{
		m_swapChainSupport = querySwapChainSupport(device.getGpu().physicalDevice, surface);
		assert(!m_swapChainSupport.availableFormats.empty());

		m_swapChainSupport.selectedFormat = chooseSwapSurfaceFormat(m_swapChainSupport.availableFormats);

		vk::Extent2D swapchainExtent{};
		int width{}, height{}; // NOLINT
		glfwGetFramebufferSize(window.getHandle(), &width, &height);

		if (m_swapChainSupport.capabilities.currentExtent.width == std::numeric_limits<u32>::max())
		{
			// If the surface size is undefined, the size is set to the size of the images requested.
			swapchainExtent.width =
				std::clamp(static_cast<u32>(width), m_swapChainSupport.capabilities.minImageExtent.width, m_swapChainSupport.capabilities.maxImageExtent.width);
			swapchainExtent.height = std::clamp(
				static_cast<u32>(height), m_swapChainSupport.capabilities.minImageExtent.height, m_swapChainSupport.capabilities.maxImageExtent.height);
		}
		else
		{
			// If the surface size is defined, the swap chain size must match
			swapchainExtent = m_swapChainSupport.capabilities.currentExtent;
		}

		auto maxImageCount = m_swapChainSupport.capabilities.maxImageCount;

		if (maxImageCount == 0) { maxImageCount = std::numeric_limits<u32>::max(); }

		m_swapChainInfo = vk::SwapchainCreateInfoKHR(
			vk::SwapchainCreateFlagsKHR(),
			surface,
			std::clamp(3U, m_swapChainSupport.capabilities.minImageCount, maxImageCount),
			m_swapChainSupport.selectedFormat.format,
			m_swapChainSupport.selectedFormat.colorSpace,
			swapchainExtent,
			1,
			vk::ImageUsageFlagBits::eColorAttachment,
			vk::SharingMode::eExclusive,
			{},
			vk::SurfaceTransformFlagBitsKHR::eIdentity,
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			m_swapChainSupport.selectedPresentMode,
			vk::False,
			nullptr);

		m_swapChain = device.getDevice().createSwapchainKHRUnique(m_swapChainInfo);
	}

	void Swapchain::createImageViews(const Device & device)
	{
		m_swapChainImages = device.getDevice().getSwapchainImagesKHR(m_swapChain.get());

		m_swapChainImageViews.reserve(m_swapChainImages.size());
		vk::ImageViewCreateInfo imageViewCreateInfo(
			{}, {}, vk::ImageViewType::e2D, m_swapChainSupport.selectedFormat.format, {}, {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

		for (auto image : m_swapChainImages)
		{
			imageViewCreateInfo.image = image;
			m_swapChainImageViews.push_back(device.getDevice().createImageViewUnique(imageViewCreateInfo));
		}
	}

	SwapChainSupportDetails Swapchain::querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface)
	{
		SwapChainSupportDetails details;
		details.capabilities		  = device.getSurfaceCapabilitiesKHR(surface);
		details.availableFormats	  = device.getSurfaceFormatsKHR(surface);
		details.availablePresentModes = device.getSurfacePresentModesKHR(surface);
		details.selectedPresentMode	  = chooseSwapPresentMode(details.availablePresentModes, desiredPresentMode_v);
		return details;
	}

	vk::SurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> & availableFormats)
	{
		for (const auto & availableFormat : availableFormats)
		{
			if (availableFormat.colorSpace != vk::ColorSpaceKHR::eSrgbNonlinear) { continue; }

			if (std::ranges::find(desiredSrgbFormats_v, availableFormat.format) != desiredSrgbFormats_v.end()) { return availableFormat; }
		}

		// Currently we only support sRGB formats, so if we can't find one, we'll just throw an error.
		throw gen::vulkan_error("Failed to find suitable surface format that supports SRGB!");
	}

	vk::PresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, vk::PresentModeKHR preferredMode)
	{
		bool doWeHaveRelaxedFifo{false};
		for (const auto & availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == preferredMode) { return availablePresentMode; }
			if (availablePresentMode == vk::PresentModeKHR::eFifoRelaxed) { doWeHaveRelaxedFifo = true; }
		}

		// If we are unable to find the preferred mode, but we have relaxed fifo, we'll just use that.
		if (doWeHaveRelaxedFifo) { return vk::PresentModeKHR::eFifoRelaxed; }

		// If we can't find the preferred mode, and we don't have relaxed fifo, the standard ensures that we will always have fifo.
		return vk::PresentModeKHR::eFifo;
	}

} // namespace gen