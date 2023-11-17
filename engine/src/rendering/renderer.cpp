// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/renderer.hpp"
#include "gen/rendering/utils/exceptions.hpp"
#include "gen/windowing/window.hpp"

namespace gen
{
	namespace
	{
		bool isDepthOnlyFormat(vk::Format format)
		{
			return format == vk::Format::eD32Sfloat || format == vk::Format::eD16Unorm;
		}

		bool isDepthStencilFormat(vk::Format format)
		{
			return format == vk::Format::eD32Sfloat || format == vk::Format::eD24UnormS8Uint || format == vk::Format::eD16Unorm;
		}

		vk::Format pickDepthFormat(vk::PhysicalDevice physicalDevice, bool depthOnly = false)
		{
			std::vector<vk::Format> const desiredFormats = {
				vk::Format::eD32Sfloat,
				vk::Format::eD24UnormS8Uint,
				vk::Format::eD16Unorm,
			};

			for (const auto & format : desiredFormats)
			{
				vk::FormatProperties const formatProperties = physicalDevice.getFormatProperties(format);

				if (depthOnly && isDepthOnlyFormat(format) && formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
				{
					return format;
				}

				if (!depthOnly && isDepthStencilFormat(format) && formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
				{
					return format;
				}
			}

			throw VulkanException("Could not find a matching depth format!");
		}

		std::vector<vk::Image> createCompleteImage(vk::PhysicalDevice gpu, vk::Format selectedFormat, vk::Extent2D surfaceExtent, vk::Image img)
		{
			vk::ImageCreateInfo const depthImageCreateInfo{
				{},
				vk::ImageType::e2D,
				selectedFormat,
				{surfaceExtent.width, surfaceExtent.height, 1},
				1,
				1,
				vk::SampleCountFlagBits::e1,
				vk::ImageTiling::eOptimal,
				vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment,
				vk::SharingMode::eExclusive,
				0,
				nullptr,
				vk::ImageLayout::eUndefined,
			};

			vk::Image const depthImg = Device::self().getHandle().createImage(depthImageCreateInfo);

			std::vector<vk::Image> combinedImage;
			combinedImage.push_back(img);
			combinedImage.push_back(depthImg);

			return combinedImage;
		}
	} // namespace

	Renderer::Renderer(const char * appName, u32 appVersion, const char * engineName, const u32 & apiVersion)
	{

		m_instance = std::make_unique<Instance>(appName, appVersion, engineName, apiVersion);
		m_device   = std::make_unique<Device>(m_instance->getHandle());

		vk::SurfaceCapabilitiesKHR const surfaceCapabilities = m_device->getGpu().physicalDevice.getSurfaceCapabilitiesKHR(m_device->getSurface());

		auto desiredPresentMode = vk::PresentModeKHR::eMailbox;

		if (Device::self().getSurface() != nullptr)
		{
			m_swapchain		= std::make_unique<Swapchain>(desiredPresentMode);
			m_surfaceExtent = m_swapchain->getExtent();
		}
		else
		{
			// From my understanding currentExtent should have the most recent valid extent.
			m_surfaceExtent = m_device->getGpu().physicalDevice.getSurfaceCapabilitiesKHR(m_device->getSurface()).currentExtent;
		}
	}

	void Renderer::render()
	{
	}

	void Renderer::prepare()
	{
		m_device->getHandle().waitIdle();

		if (m_swapchain)
		{
			vk::Extent2D const surfaceExtent = m_swapchain->getExtent();

			for (const auto & img : m_swapchain->getImages())
			{
				auto combinedImage =
					createCompleteImage(m_device->getGpu().physicalDevice, pickDepthFormat(m_device->getGpu().physicalDevice), surfaceExtent, img);

				m_frames.emplace_back(std::make_unique<Frame>(std::move(combinedImage)));
			}
		}
		else
		{
			vk::Extent2D const surfaceExtent = m_device->getGpu().physicalDevice.getSurfaceCapabilitiesKHR(m_device->getSurface()).currentExtent;

			auto colorImageCreateInfo = vk::ImageCreateInfo{
				{},
				vk::ImageType::e2D,
				vk::Format::eR8G8B8A8Srgb,
				{surfaceExtent.width, surfaceExtent.height, 1},
				1,
				1,
				vk::SampleCountFlagBits::e1,
				vk::ImageTiling::eOptimal,
				vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
				vk::SharingMode::eExclusive,
				0,
				nullptr,
				vk::ImageLayout::eUndefined,
			};

			vk::Image const colorImg = m_device->getHandle().createImage(colorImageCreateInfo);
			m_frames.emplace_back(std::make_unique<Frame>(std::vector<vk::Image>{colorImg}));
		}
	}

} // namespace gen