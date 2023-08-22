// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

// TODO: Replace this with a proper implementation

#include "application.hpp"
#include <numbers>

#include <graphics/graphicsExceptions.hpp>

namespace gen
{
	namespace
	{
		struct ImageBarrier
		{
			vk::ImageMemoryBarrier2 barrier{};

			explicit ImageBarrier(vk::Image image, std::uint32_t mip_levels = 1, std::uint32_t array_layers = 1);

			auto set_full_barrier(vk::ImageLayout src, vk::ImageLayout dst) -> ImageBarrier &;
			auto set_undef_to_optimal(bool depth) -> ImageBarrier &;
			auto set_optimal_to_present() -> ImageBarrier &;

			auto transition(vk::CommandBuffer cmd) const -> void;
			static auto transition(vk::CommandBuffer cmd, std::span<vk::ImageMemoryBarrier2 const> barriers) -> void;
		};

		ImageBarrier::ImageBarrier(vk::Image image, std::uint32_t mip_levels, std::uint32_t array_layers)
		{
			barrier.image			 = image;
			barrier.subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, mip_levels, 0, array_layers};
		}

		auto ImageBarrier::set_full_barrier(vk::ImageLayout src, vk::ImageLayout dst) -> ImageBarrier &
		{
			barrier.oldLayout	 = src;
			barrier.newLayout	 = dst;
			barrier.srcStageMask = barrier.dstStageMask = vk::PipelineStageFlagBits2::eAllCommands;
			barrier.srcAccessMask = barrier.dstAccessMask = vk::AccessFlagBits2::eMemoryRead | vk::AccessFlagBits2::eMemoryWrite;
			return *this;
		}

		auto ImageBarrier::set_undef_to_optimal(bool depth) -> ImageBarrier &
		{
			barrier.oldLayout	  = vk::ImageLayout::eUndefined;
			barrier.newLayout	  = vk::ImageLayout::eAttachmentOptimal;
			barrier.srcStageMask  = vk::PipelineStageFlagBits2::eNone;
			barrier.srcAccessMask = vk::AccessFlagBits2::eNone;
			if (depth)
			{
				barrier.dstStageMask				= vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests;
				barrier.dstAccessMask				= vk::AccessFlagBits2::eDepthStencilAttachmentWrite | vk::AccessFlagBits2::eDepthStencilAttachmentRead;
				barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
			}
			else
			{
				barrier.dstStageMask  = vk::PipelineStageFlagBits2::eColorAttachmentOutput;
				barrier.dstAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite | vk::AccessFlagBits2::eColorAttachmentRead;
			}
			return *this;
		}

		auto ImageBarrier::set_optimal_to_present() -> ImageBarrier &
		{
			barrier.oldLayout	  = vk::ImageLayout::eAttachmentOptimal;
			barrier.newLayout	  = vk::ImageLayout::ePresentSrcKHR;
			barrier.srcStageMask  = vk::PipelineStageFlagBits2::eColorAttachmentOutput;
			barrier.srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite | vk::AccessFlagBits2::eColorAttachmentRead;
			barrier.dstStageMask  = vk::PipelineStageFlagBits2::eNone;
			barrier.dstAccessMask = vk::AccessFlagBits2::eNone;
			return *this;
		}

		auto ImageBarrier::transition(vk::CommandBuffer cmd) const -> void
		{
			if (!barrier.image) { return; }
			transition(cmd, {&barrier, 1});
		}

		auto ImageBarrier::transition(vk::CommandBuffer cmd, std::span<vk::ImageMemoryBarrier2 const> barriers) -> void
		{
			if (barriers.empty()) { return; }
			auto vdi					= vk::DependencyInfo{};
			vdi.imageMemoryBarrierCount = static_cast<std::uint32_t>(barriers.size());
			vdi.pImageMemoryBarriers	= barriers.data();
			cmd.pipelineBarrier2(vdi);
		}

	} // namespace
	// possibly change this to instead use Update() and Draw() functions

	void Application::run()
	{
		auto device	  = m_graphicsDevice.getDevice();
		m_commandPool = device.createCommandPoolUnique({vk::CommandPoolCreateFlagBits::eResetCommandBuffer | vk::CommandPoolCreateFlagBits::eTransient});
		auto cbai	  = vk::CommandBufferAllocateInfo{*m_commandPool};
		cbai.commandBufferCount = 1;
		if (device.allocateCommandBuffers(&cbai, &m_sync.cmd) != vk::Result::eSuccess) { throw vulkan_error{"failed to allocate command buffer"}; }

		m_sync.acquire = device.createSemaphoreUnique({});
		m_sync.present = device.createSemaphoreUnique({});
		m_sync.ready   = device.createFenceUnique({vk::FenceCreateFlagBits::eSignaled});

		gameLoop();
		shutdown();
	}

	void Application::gameLoop()
	{
		while (!m_window.shouldClose())
		{
			poll();
			// TODO: tick(dt)
			render();
		}
	}

	void Application::poll() // NOLINT
	{
		Window::pollEvents();
		// TODO: update input state (keys pressed/held/released, cursor position, window and fb size, etc)
	}

	void Application::render()
	{
		auto device	   = m_graphicsDevice.getDevice();
		auto swapchain = m_graphicsDevice.getSwapchain();

		// wait for frame
		device.waitForFences(*m_sync.ready, vk::True, std::numeric_limits<std::uint64_t>::max()); // TODO handle error
		device.resetFences(*m_sync.ready);

		auto const handleResult = [&](vk::Result const result)
		{
			switch (result)
			{
			case vk::Result::eSuboptimalKHR:
			case vk::Result::eErrorOutOfDateKHR:
			{
				// TODO: recreate swapchain
				break;
			}
			case vk::Result::eSuccess: break;
			default: throw vulkan_error{"failed to acquire swapchain image"};
			}
		};

		// acquire image
		auto acquire = device.acquireNextImageKHR(swapchain.swapchain, std::numeric_limits<std::uint64_t>::max(), *m_sync.acquire);
		handleResult(acquire.result);
		auto const imageIndex = acquire.value;
		assert(imageIndex < swapchain.images.size());

		// render
		m_sync.cmd.begin({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
		// transition for draw
		auto imageBarrier = ImageBarrier{swapchain.images[imageIndex]};
		imageBarrier.set_undef_to_optimal(false);
		imageBarrier.transition(m_sync.cmd);

		// draw
		auto rai		= vk::RenderingAttachmentInfo{};
		rai.imageLayout = vk::ImageLayout::eAttachmentOptimal;
		rai.imageView	= swapchain.imageViews[imageIndex];
		rai.clearValue	= vk::ClearColorValue{1.0f, 0.0f, 0.0f, 1.0f};
		rai.loadOp		= vk::AttachmentLoadOp::eClear;
		rai.storeOp		= vk::AttachmentStoreOp::eStore;

		auto ri					= vk::RenderingInfo{};
		ri.renderArea.extent	= vk::Extent2D{800, 600};
		ri.renderArea.offset	= vk::Offset2D{};
		ri.colorAttachmentCount = 1;
		ri.pColorAttachments	= &rai;
		ri.layerCount			= 1;
		m_sync.cmd.beginRendering(ri);
		// bind pipeline, draw stuff
		m_sync.cmd.endRendering();

		// transition for present
		imageBarrier.set_optimal_to_present();
		imageBarrier.transition(m_sync.cmd);

		m_sync.cmd.end();

		// submit
		auto const cbi		 = vk::CommandBufferSubmitInfo{m_sync.cmd};
		auto const ssiWait	 = vk::SemaphoreSubmitInfo{*m_sync.acquire, {}, vk::PipelineStageFlagBits2::eColorAttachmentOutput};
		auto const ssiSignal = vk::SemaphoreSubmitInfo{*m_sync.present, {}, vk::PipelineStageFlagBits2::eColorAttachmentOutput};

		auto si						= vk::SubmitInfo2{};
		si.commandBufferInfoCount	= 1;
		si.pCommandBufferInfos		= &cbi;
		si.waitSemaphoreInfoCount	= 1;
		si.pWaitSemaphoreInfos		= &ssiWait;
		si.signalSemaphoreInfoCount = 1;
		si.pSignalSemaphoreInfos	= &ssiSignal;

		auto queue = m_graphicsDevice.getQueue();
		queue.submit2(si, *m_sync.ready);

		// present
		auto pi				  = vk::PresentInfoKHR{};
		pi.pWaitSemaphores	  = &*m_sync.present;
		pi.waitSemaphoreCount = 1;
		pi.pImageIndices	  = &imageIndex;
		pi.pSwapchains		  = &swapchain.swapchain;
		pi.swapchainCount	  = 1;
		auto const present	  = queue.presentKHR(pi);
		handleResult(present);
	}

	void Application::shutdown()
	{
		m_graphicsDevice.getDevice().waitIdle();
	}

	Application::Application(const char * appName, mim::vec2i const & initialSize) : m_window{initialSize, appName}, m_graphicsDevice{m_window, appName}
	{
	}

} // namespace gen
