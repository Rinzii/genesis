// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/swapchain.hpp"
#include "gen/graphics/graphicsExceptions.hpp"

#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

namespace gen
{

	namespace
	{
		ImageBarrier::ImageBarrier(vk::Image image, u32 mipLevels, u32 arrayLayers)
		{
			m_barrier.image			   = image;
			m_barrier.subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, mipLevels, 0, arrayLayers};
		}

		ImageBarrier & ImageBarrier::setFullBarrier(vk::ImageLayout src, vk::ImageLayout dst)
		{
			m_barrier.oldLayout	   = src;
			m_barrier.newLayout	   = dst;
			m_barrier.srcStageMask = m_barrier.dstStageMask = vk::PipelineStageFlagBits2::eAllCommands;
			m_barrier.srcAccessMask = m_barrier.dstAccessMask = vk::AccessFlagBits2::eMemoryRead | vk::AccessFlagBits2::eMemoryWrite;

			return *this;
		}

		ImageBarrier & ImageBarrier::setUndefToOptimal(bool depth)
		{

			m_barrier.oldLayout		= vk::ImageLayout::eUndefined;
			m_barrier.newLayout		= vk::ImageLayout::eAttachmentOptimal;
			m_barrier.srcStageMask	= vk::PipelineStageFlagBits2::eNone;
			m_barrier.srcAccessMask = vk::AccessFlagBits2::eNone;
			if (depth)
			{
				m_barrier.dstStageMask				  = vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests;
				m_barrier.dstAccessMask				  = vk::AccessFlagBits2::eDepthStencilAttachmentWrite | vk::AccessFlagBits2::eDepthStencilAttachmentRead;
				m_barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
			}
			else
			{
				m_barrier.dstStageMask	= vk::PipelineStageFlagBits2::eColorAttachmentOutput;
				m_barrier.dstAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite | vk::AccessFlagBits2::eColorAttachmentRead;
			}

			return *this;
		}

		ImageBarrier & ImageBarrier::setOptimalToPresent()
		{
			m_barrier.oldLayout		= vk::ImageLayout::eAttachmentOptimal;
			m_barrier.newLayout		= vk::ImageLayout::ePresentSrcKHR;
			m_barrier.srcStageMask	= vk::PipelineStageFlagBits2::eColorAttachmentOutput;
			m_barrier.srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite | vk::AccessFlagBits2::eColorAttachmentRead;
			m_barrier.dstStageMask	= vk::PipelineStageFlagBits2::eNone;
			m_barrier.dstAccessMask = vk::AccessFlagBits2::eNone;
			return *this;
		}

		void ImageBarrier::transition(vk::CommandBuffer cmd)
		{
			if (!m_barrier.image) { return; }
			transition(cmd, {&m_barrier, 1});
		}

		void ImageBarrier::transition(vk::CommandBuffer cmd, std::span<const vk::ImageMemoryBarrier2> barriers)
		{
			if (barriers.empty()) { return; }
			auto vdi					= vk::DependencyInfo{};
			vdi.imageMemoryBarrierCount = static_cast<std::uint32_t>(barriers.size());
			vdi.pImageMemoryBarriers	= barriers.data();
			cmd.pipelineBarrier2(vdi);
		}

	} // namespace

	constexpr auto desiredSrgbFormats_v				  = std::array{vk::Format::eB8G8R8A8Srgb, vk::Format::eR8G8B8A8Srgb};
	constexpr vk::PresentModeKHR desiredPresentMode_v = {vk::PresentModeKHR::eMailbox};

	Swapchain::Swapchain(const Window & window)
	{

		createSwapChain(window, Device::self());
		createImageViews(Device::self());
		createSyncObjects(Device::self());
		m_logger.info("Swapchain created");
	}

	Swapchain::~Swapchain()
	{
		m_logger.info("Swapchain destructed");
	}

	void Swapchain::createSwapChain(const Window & window, const Device & device)
	{
		m_swapChainSupport = querySwapChainSupport(device.getGpu().physicalDevice, device.getSurface());
		assert(!m_swapChainSupport.availableFormats.empty());

		m_swapChainSupport.selectedFormat = chooseSwapSurfaceFormat(m_swapChainSupport.availableFormats);

		vk::Extent2D const swapchainExtent = chooseSwapExtent(m_swapChainSupport, window);

		auto maxImageCount = m_swapChainSupport.capabilities.maxImageCount;

		if (maxImageCount == 0) { maxImageCount = std::numeric_limits<u32>::max(); }

		m_swapChainInfo = vk::SwapchainCreateInfoKHR(
			vk::SwapchainCreateFlagsKHR(),
			device.getSurface(),
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

	void Swapchain::createSyncObjects(const Device & device)
	{
		m_commandPool =
			device.getDevice().createCommandPoolUnique({vk::CommandPoolCreateFlagBits::eResetCommandBuffer | vk::CommandPoolCreateFlagBits::eTransient});
		auto cmdBufAllocInfo				= vk::CommandBufferAllocateInfo(*m_commandPool);
		cmdBufAllocInfo.commandBufferCount = static_cast<u32>(m_frames.sync.size());

		std::array<vk::CommandBuffer, bufferingCount_v> cmdBuffers;

		if (device.getDevice().allocateCommandBuffers(&cmdBufAllocInfo, cmdBuffers.data()) != vk::Result::eSuccess)
		{
			throw gen::vulkan_error("Failed to allocate command buffers!");
		}

		for (std::size_t i = 0; i < m_frames.sync.size(); ++i)
		{
			m_frames.sync[i].cmd	= cmdBuffers[i]; // NOLINT
			m_frames.sync[i].image	= device.getDevice().createSemaphoreUnique({});
			m_frames.sync[i].render	= device.getDevice().createSemaphoreUnique({});
			m_frames.sync[i].inFlight = device.getDevice().createFenceUnique({vk::FenceCreateFlagBits::eSignaled});
		}
	}

	void Swapchain::recreateSwapChain(const Window & window, const Device & device)
	{
		device.getDevice().waitIdle();

		createSwapChain(window, device);
		createImageViews(device);
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
	vk::Extent2D Swapchain::chooseSwapExtent(const SwapChainSupportDetails & swapChainSupportDetails, const Window & window)
	{
		vk::Extent2D swapchainExtent;
		int width{}, height{}; // NOLINT
		glfwGetFramebufferSize(window.getHandle(), &width, &height);

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
		}

		return swapchainExtent;
	}

} // namespace gen