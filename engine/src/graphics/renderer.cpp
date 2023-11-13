// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/renderer.hpp"

#include <vulkan/vulkan.hpp>

#include "gen/graphics/graphicsExceptions.hpp"
#include "gen/graphics/vkHelpers.hpp"

namespace gen
{

	Renderer::Renderer(const char * appName, u32 appVersion, const char * engineName, const u32 & apiVersion)
	{
		createInstance(appName, appVersion, engineName, apiVersion);

		m_device = std::make_unique<Device>(m_instance.get());

		prepare();

		m_logger.debug("Renderer constructed");
	}

	void Renderer::render()
	{
		if (!m_prepared) { return; }

		renderFrame();
	}

	void Renderer::windowResize()
	{
		if (!m_prepared) { return; }
		m_prepared = false;
		m_resized  = true;

		m_device->getDevice().waitIdle();

		// Recreate swap chain
		glfwGetFramebufferSize(Window::self().getHandle(), &m_framebufferExtent.x, &m_framebufferExtent.y);
		setupSwapChain();

		// recreate the command buffers
		createCommandBuffers();
		buildCommandBuffers();
	}

	void Renderer::renderFrame()
	{
		prepareFrame(getCurrentFrame());
		m_submitInfo.commandBufferCount = 1;
		m_submitInfo.pCommandBuffers	= &m_frames[m_currentBuffer].cmd.get();
		m_device->submit(m_submitInfo);
		submitFrame(getCurrentFrame());
	}

	void Renderer::prepareFrame(FrameData & frame)
	{
		vk::utils::checkResult(m_device->getDevice().waitForFences(1, &frame.renderCompleteFence.get(), vk::True, std::numeric_limits<uint64_t>::max()));
		vk::utils::checkResult(m_device->getDevice().resetFences(1, &frame.renderCompleteFence.get()));

		vk::Result result = m_swapChain->acquireNextImage(frame.presentCompleteSemaphore.get(), &m_currentBuffer);

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) { windowResize(); }
		else { vk::utils::checkResult(result); }
	}

	void Renderer::submitFrame(FrameData & frame)
	{
		vk::PipelineStageFlags submitWaitStates = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		m_submitInfo							= vk::SubmitInfo();
		m_submitInfo.pWaitDstStageMask			= &submitWaitStates;
		m_submitInfo.waitSemaphoreCount			= 1;
		m_submitInfo.pWaitSemaphores			= &frame.presentCompleteSemaphore.get();
		m_submitInfo.signalSemaphoreCount		= 1;
		m_submitInfo.pSignalSemaphores			= &frame.renderCompleteSemaphore.get();
		m_submitInfo.commandBufferCount			= 1;
		m_submitInfo.pCommandBuffers			= &frame.cmd.get();

		// TODO: Getting seg fault here
		m_device->submit(m_submitInfo, frame.renderCompleteFence.get());

		vk::Result result = m_swapChain->queuePresent(m_device->getQueue(), m_currentBuffer, frame.renderCompleteSemaphore.get());

		if (!(result == vk::Result::eSuccess || result == vk::Result::eSuboptimalKHR))
		{
			if (result == vk::Result::eErrorOutOfDateKHR) { windowResize(); }
			else { vk::utils::checkResult(result); }
		}

		m_device->getQueue().waitIdle();

		m_frameIndex++;
		if (m_frameIndex > m_renderAhead) { m_frameIndex = 0; }
	}

	void Renderer::prepare()
	{
		m_device->getDevice().waitIdle();

		initSwapChain();
		createCommandPool();
		setupSwapChain();
		createCommandBuffers();
		createSyncStructures();
		setupDepthStencil();

		buildCommandBuffers();

		m_prepared = true;
	}

	void Renderer::createInstance(const char * appName, u32 appVersion, const char * engineName, const u32 & apiVersion)
	{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const dynLoader;
		auto vkGetInstanceProcAddr = dynLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		vk::ApplicationInfo const appInfo(appName, appVersion, engineName, gen::version_v.getVersion(), apiVersion);

		auto extensionsCount	   = 0U;
		auto * requestedExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
		std::vector<std::string> const requestedExtensionsVec(
			requestedExtensions,
			requestedExtensions + extensionsCount); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		auto enabledExtensions = vk::utils::gatherExtensions(
			requestedExtensionsVec
#ifndef GEN_NDEBUG
			,
			vk::enumerateInstanceExtensionProperties()
#endif
		);

		m_instance = vk::createInstanceUnique(vk::utils::makeInstanceCreateInfoChain(appInfo, {}, enabledExtensions).get<vk::InstanceCreateInfo>());

		if (!m_instance) { throw gen::VulkanException("Failed to create instance!"); }

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);
#endif
	}

	void Renderer::initSwapChain()
	{
		if (m_device->getSurface() != nullptr)
		{
			m_swapChain			= std::make_unique<SwapChain>(m_device->getSurface());
			m_framebufferExtent = m_swapChain->getExtent();
		}
		else
		{
			mim::vec2i framebufferExtent;
			glfwGetFramebufferSize(Window::self().getHandle(), &framebufferExtent.x, &framebufferExtent.y);

			m_framebufferExtent.width  = static_cast<u32>(framebufferExtent.x);
			m_framebufferExtent.height = static_cast<u32>(framebufferExtent.y);
		}
	}

	void Renderer::createCommandPool()
	{
		vk::CommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.queueFamilyIndex		  = m_device->getQueueFamily();
		cmdPoolInfo.flags					  = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
		m_commandPool						  = m_device->getDevice().createCommandPoolUnique(cmdPoolInfo);
	}

	void Renderer::setupSwapChain()
	{
		glfwGetFramebufferSize(Window::self().getHandle(), &m_framebufferExtent.x, &m_framebufferExtent.y);
		m_swapChain->create(static_cast<u32>(m_framebufferExtent.x), static_cast<u32>(m_framebufferExtent.y), m_settings.vsync);
	}

	void Renderer::createCommandBuffers()
	{
		// We will create one command buffer for each swap chain image and reuse for rendering
		auto cmdBufAllocateInfo = vk::CommandBufferAllocateInfo()
									  .setCommandPool(m_commandPool.get())
									  .setLevel(vk::CommandBufferLevel::ePrimary)
									  .setCommandBufferCount(static_cast<u32>(m_frames.size()));

		for (auto & frame : m_frames) { frame.cmd = std::move(m_device->getDevice().allocateCommandBuffersUnique(cmdBufAllocateInfo)[0]); }
	}

	void Renderer::createSyncStructures()
	{

		// Wait fences for syncing command buffer access
		vk::FenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.flags				= vk::FenceCreateFlagBits::eSignaled;

		for (auto & frame : m_frames) { frame.renderCompleteFence = m_device->getDevice().createFenceUnique(fenceCreateInfo); }
	}

	void Renderer::setupDepthStencil()
	{
		// Find a suitable depth format
		// All depth formats may be optional so have to find a suitable depth format to use
		// Starting with the highest precision packed format

		std::vector<vk::Format> depthFormats = {
			vk::Format::eD32SfloatS8Uint, vk::Format::eD32Sfloat, vk::Format::eD24UnormS8Uint, vk::Format::eD16UnormS8Uint, vk::Format::eD16Unorm};

		for (auto & format : depthFormats)
		{
			vk::FormatProperties formatProps = m_device->getPhysicalDevice().getFormatProperties(format);
			// Format must support depth stencil attachment for optimal tiling
			if (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
			{
				m_depthFormat = format;
				break;
			}
		}
		m_logger.debug("Using depth format: {}", vk::to_string(m_depthFormat));

		vk::ImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.imageType			= vk::ImageType::e2D;
		imageCreateInfo.format				= m_depthFormat;
		imageCreateInfo.extent				= vk::Extent3D{static_cast<uint32_t>(m_framebufferExtent.x), static_cast<uint32_t>(m_framebufferExtent.y), 1};
		imageCreateInfo.mipLevels			= 1;
		imageCreateInfo.arrayLayers			= 1;
		imageCreateInfo.samples				= vk::SampleCountFlagBits::e1;
		imageCreateInfo.tiling				= vk::ImageTiling::eOptimal;
		imageCreateInfo.usage				= vk::ImageUsageFlagBits::eDepthStencilAttachment;

		m_depthStencil.image = m_device->getDevice().createImageUnique(imageCreateInfo);

		vk::MemoryRequirements memReqs = m_device->getDevice().getImageMemoryRequirements(m_depthStencil.image.get());

		vk::MemoryAllocateInfo memAllocInfo = {};
		memAllocInfo.allocationSize			= memReqs.size;
		memAllocInfo.memoryTypeIndex		= m_device->getMemoryType(memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
		m_depthStencil.mem					= m_device->getDevice().allocateMemoryUnique(memAllocInfo);

		m_device->getDevice().bindImageMemory(m_depthStencil.image.get(), m_depthStencil.mem.get(), 0);

		vk::ImageViewCreateInfo imageViewCreateInfo			= {};
		imageViewCreateInfo.viewType						= vk::ImageViewType::e2D;
		imageViewCreateInfo.image							= m_depthStencil.image.get();
		imageViewCreateInfo.format							= m_depthFormat;
		imageViewCreateInfo.subresourceRange.baseMipLevel	= 0;
		imageViewCreateInfo.subresourceRange.levelCount		= 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount		= 1;
		imageViewCreateInfo.subresourceRange.aspectMask		= vk::ImageAspectFlagBits::eDepth;

		if (m_depthFormat >= vk::Format::eD16UnormS8Uint) { imageViewCreateInfo.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil; }

		m_depthStencil.view = m_device->getDevice().createImageViewUnique(imageViewCreateInfo);
	}

	void Renderer::buildCommandBuffers()
	{
		vk::CommandBufferBeginInfo cmdBufInfo = {};

		for (u32 i = 0; i < m_frames.size(); ++i)
		{
			m_frames[i].cmd->begin(cmdBufInfo);

			// Begin the transition of the color and depth images
			vk::utils::insertImageMemoryBarrier(
				m_frames[i].cmd.get(),
				m_swapChain->getBufferAt(i).image,
				{},
				vk::AccessFlagBits::eColorAttachmentWrite,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::ePresentSrcKHR,
				vk::PipelineStageFlagBits::eBottomOfPipe,
				vk::PipelineStageFlagBits::eBottomOfPipe,
				vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

			vk::utils::insertImageMemoryBarrier(
				m_frames[i].cmd.get(),
				m_depthStencil.image.get(),
				{},
				vk::AccessFlagBits::eDepthStencilAttachmentWrite,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::eDepthStencilAttachmentOptimal,
				vk::PipelineStageFlagBits::eBottomOfPipe,
				vk::PipelineStageFlagBits::eBottomOfPipe,
				vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1});

			// vk::ClearValue clearValue;
			// float flash = abs(sin(m_currentBuffer / 120.f));
			// clearValue.color = vk::ClearColorValue {std::array{0.0F, 0.0F, flash, 1.0F}};

			vk::RenderingAttachmentInfoKHR colorAttachment{};
			colorAttachment.imageView	= m_swapChain->getBufferAt(i).view.get();
			colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
			colorAttachment.loadOp		= vk::AttachmentLoadOp::eClear;
			colorAttachment.storeOp		= vk::AttachmentStoreOp::eStore;
			colorAttachment.clearValue	= vk::ClearColorValue{std::array{1.0F, 1.0F, 0.0F, 1.0F}};

			vk::RenderingAttachmentInfoKHR depthStencilAttachment{};
			depthStencilAttachment.imageView   = m_depthStencil.view.get();
			depthStencilAttachment.imageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
			depthStencilAttachment.loadOp	   = vk::AttachmentLoadOp::eClear;
			depthStencilAttachment.storeOp	   = vk::AttachmentStoreOp::eStore;
			depthStencilAttachment.clearValue  = vk::ClearDepthStencilValue{1.0F, 0};

			glfwGetFramebufferSize(Window::self().getHandle(), &m_framebufferExtent.x, &m_framebufferExtent.y);

			vk::RenderingInfoKHR renderingInfo{};
			renderingInfo.renderArea		   = vk::Rect2D{{0, 0}, {static_cast<u32>(m_framebufferExtent.x), static_cast<u32>(m_framebufferExtent.y)}};
			renderingInfo.layerCount		   = 1;
			renderingInfo.colorAttachmentCount = 1;
			renderingInfo.pColorAttachments	   = &colorAttachment;
			renderingInfo.pDepthAttachment	   = &depthStencilAttachment;
			renderingInfo.pStencilAttachment   = &depthStencilAttachment;

			// Start dynamic rendering
			m_frames[i].cmd->beginRenderingKHR(renderingInfo);

			vk::Viewport viewport{};
			viewport.width	  = static_cast<float>(m_framebufferExtent.x);
			viewport.height	  = static_cast<float>(m_framebufferExtent.y);
			viewport.minDepth = 0.0F;
			viewport.maxDepth = 1.0F;

			// m_drawCommandBuffers[i]->setViewport(0, viewport);

			// End dynamic rendering
			m_frames[i].cmd->endRenderingKHR();

			// Now transition the color image for presentation
			vk::utils::insertImageMemoryBarrier(
				m_frames[i].cmd.get(),
				m_swapChain->getBufferAt(i).image,
				vk::AccessFlagBits::eColorAttachmentWrite,
				{},
				vk::ImageLayout::eColorAttachmentOptimal,
				vk::ImageLayout::ePresentSrcKHR,
				vk::PipelineStageFlagBits::eColorAttachmentOutput,
				vk::PipelineStageFlagBits::eBottomOfPipe,
				vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

			m_frames[i].cmd->end();
		}
	}
	Renderer::FrameData & Renderer::getCurrentFrame()
	{
		return m_frames[m_frameIndex % m_renderAhead];
	}

} // namespace gen

// namespace gen