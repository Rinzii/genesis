// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/swapchain.hpp"
#include "gen/rendering/device.hpp"
#include "gen/rendering/utils/exceptions.hpp"

namespace gen
{
	namespace
	{
		SwapchainSupportDetails querySwapChainSupport(Device & device, vk::SurfaceKHR surface)
		{
			SwapchainSupportDetails details;

			details.capabilities = device.getGpu().physicalDevice.getSurfaceCapabilitiesKHR(surface);
			details.formats		 = device.getGpu().physicalDevice.getSurfaceFormatsKHR(surface);
			details.presentModes = device.getGpu().physicalDevice.getSurfacePresentModesKHR(surface);

			return details;
		}

		u32 selectImageCount(u32 requestedImageCount, u32 minImageCount, u32 maxImageCount)
		{
			requestedImageCount = std::min(requestedImageCount, maxImageCount);
			requestedImageCount = std::max(requestedImageCount, minImageCount);

			return requestedImageCount;
		}

		vk::Extent2D selectExtent(vk::Extent2D requestedExtent, vk::Extent2D minExtent, vk::Extent2D maxExtent, vk::Extent2D currentExtent)
		{
			if (requestedExtent.width < 1 || requestedExtent.height < 1) { return currentExtent; }

			requestedExtent.width = std::max(requestedExtent.width, minExtent.width);
			requestedExtent.width = std::min(requestedExtent.width, maxExtent.width);

			requestedExtent.height = std::max(requestedExtent.height, minExtent.height);
			requestedExtent.height = std::min(requestedExtent.height, maxExtent.height);

			return requestedExtent;
		}

		vk::SurfaceFormatKHR selectSurfaceFormat(
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

		u32 selectImageArrayLayers(u32 requestedImageArrayLayers, u32 maxImageArrayLayers)
		{
			requestedImageArrayLayers = std::min(requestedImageArrayLayers, maxImageArrayLayers);
			requestedImageArrayLayers = std::max(requestedImageArrayLayers, 1U);

			return requestedImageArrayLayers;
		}

		vk::ImageUsageFlags selectImageUsage(vk::ImageUsageFlags requestedImageUsage, vk::ImageUsageFlags supportedImageUsage)
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

		vk::SurfaceTransformFlagBitsKHR selectPreTransform(
			vk::SurfaceTransformFlagBitsKHR requestedPreTransform,
			vk::SurfaceTransformFlagsKHR supportedPreTransform,
			vk::SurfaceTransformFlagBitsKHR currentPreTransform)
		{
			if (supportedPreTransform & requestedPreTransform) { return requestedPreTransform; }

			return currentPreTransform;
		}

		vk::CompositeAlphaFlagBitsKHR selectCompositeAlpha(
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

		vk::PresentModeKHR selectPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, vk::PresentModeKHR requestedPresentMode)
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
	} // namespace

	Swapchain::Swapchain(Swapchain & oldSwapchain, const u32 maxImageCount)
	{
	}

	Swapchain::Swapchain(Swapchain & oldSwapchain, const vk::Extent2D & extent)
	{
	}

	Swapchain::Swapchain(Swapchain & oldSwapchain, const vk::ImageUsageFlags & imageUsageFlags)
	{
	}

	Swapchain::Swapchain(Swapchain & oldSwapchain, const vk::Extent2D & extent, const vk::SurfaceTransformFlagBitsKHR preTransform)
	{
	}

	Swapchain::Swapchain(
		const vk::PresentModeKHR & desiredPresentMode,
		const std::vector<vk::PresentModeKHR> & presentModePriorityList,
		const std::vector<vk::SurfaceFormatKHR> & surfaceFormatPriorityList,
		const vk::Extent2D & extent,
		const u32 & maxImageCount,
		const vk::SurfaceTransformFlagBitsKHR & preTransform,
		const vk::ImageUsageFlags & imageUsagePriorityList)
		: Swapchain{*this, desiredPresentMode, presentModePriorityList, surfaceFormatPriorityList, extent, maxImageCount, preTransform, imageUsagePriorityList}
	{
	}

	Swapchain::Swapchain(
		Swapchain & oldSwapchain,
		const vk::PresentModeKHR & desiredPresentMode,
		const std::vector<vk::PresentModeKHR> & presentModePriorityList,
		const std::vector<vk::SurfaceFormatKHR> & surfaceFormatPriorityList,
		const vk::Extent2D & extent,
		const u32 & maxImageCount,
		const vk::SurfaceTransformFlagBitsKHR & preTransform,
		const vk::ImageUsageFlags & imageUsagePriorityList)
	{
		m_supportDetails = querySwapChainSupport(Device::self(), Device::self().getSurface());

		m_properties.surfaceFormat = selectSurfaceFormat(m_supportDetails.formats, surfaceFormatPriorityList);

		vk::SwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.flags					   = vk::SwapchainCreateFlagsKHR();
		swapchainCreateInfo.surface					   = Device::self().getSurface();
		swapchainCreateInfo.minImageCount =
			selectImageCount(maxImageCount, m_supportDetails.capabilities.minImageCount, m_supportDetails.capabilities.maxImageCount);
		swapchainCreateInfo.imageFormat		= m_properties.surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = m_properties.surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent		= selectExtent(
			extent, m_supportDetails.capabilities.minImageExtent, m_supportDetails.capabilities.maxImageExtent, m_supportDetails.capabilities.currentExtent);
		swapchainCreateInfo.imageArrayLayers = selectImageArrayLayers(1, m_supportDetails.capabilities.maxImageArrayLayers);
		swapchainCreateInfo.imageUsage		 = selectImageUsage(vk::ImageUsageFlagBits::eColorAttachment, m_supportDetails.capabilities.supportedUsageFlags);
		swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
		swapchainCreateInfo.preTransform =
			selectPreTransform(preTransform, m_supportDetails.capabilities.supportedTransforms, m_supportDetails.capabilities.currentTransform);
		swapchainCreateInfo.compositeAlpha =
			selectCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque, m_supportDetails.capabilities.supportedCompositeAlpha);
		swapchainCreateInfo.presentMode = selectPresentMode(m_supportDetails.presentModes, desiredPresentMode);

		swapchainCreateInfo.oldSwapchain = oldSwapchain.m_handle.get();

		m_properties.createInfo = swapchainCreateInfo;

		m_handle = Device::self().getHandle().createSwapchainKHRUnique(swapchainCreateInfo);

		if (m_handle.get() == nullptr) { throw gen::VulkanException("Failed to create swapchain!"); }

		m_images = Device::self().getHandle().getSwapchainImagesKHR(m_handle.get());
	}

	Swapchain::~Swapchain()
	{
	}

	std::pair<vk::Result, u32> Swapchain::acquireNextImage(const vk::Semaphore & semaphore, const vk::Fence fence)
	{
		vk::ResultValue<u32> const result = Device::self().getHandle().acquireNextImageKHR(m_handle.get(), std::numeric_limits<u64>::max(), semaphore, fence);
		return {result.result, result.value};
	}

} // namespace gen