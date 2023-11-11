// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "device.hpp"
#include "gen/core.hpp"
#include "gen/core/monoInstance.hpp"
#include "gen/logger/log.hpp"
#include "gen/windowing/window.hpp"
#include "swapchain.hpp"

#include <vulkan/vulkan.hpp>

#include <memory>

namespace gen
{
	class Renderer : public MonoInstance<Renderer>
	{
		struct FrameData;

	public:
		Renderer(const char * appName, u32 appVersion, const char * engineName, const u32 & apiVersion);
		~Renderer() = default;

		Renderer(const Renderer &)			   = delete;
		Renderer(Renderer &&)				   = delete;
		Renderer & operator=(const Renderer &) = delete;
		Renderer & operator=(Renderer &&)	   = delete;

		void render();

		void windowResize();

		/*
		void viewChanged();
		void windowResized();
		void buildCommandBuffers();
		void getEnabledFeatures();
		void getEnabledExtensions();
		vk::PipelineShaderStageCreateInfo loadShader(const std::string & fileName, vk::ShaderStageFlagBits stage);
		*/

		FrameData & getCurrentFrame();

	private:
		void renderFrame();

		void prepareFrame(FrameData & frame);

		void submitFrame(FrameData & frame);

		void createInstance(const char * appName, u32 appVersion, const char * engineName, const u32 & apiVersion);

		void prepare();

		void initSwapChain();

		void createCommandPool();

		void setupSwapChain();

		void createCommandBuffers();

		void createSyncStructures();

		void setupDepthStencil();

		/*
		void createPipelineCache();

		void prepareUniformBuffers();

		void setupDescriptorSetLayout();

		void preparePipelines();

		void setupDescriptorPool();

		void setupDescriptorSet();
		 */

		void buildCommandBuffers();

		struct Settings
		{
			bool fullscreen{false};
			bool vsync{false};

		} m_settings{};

		vk::UniqueInstance m_instance{};

		std::unique_ptr<Device> m_device{};

		std::unique_ptr<SwapChain> m_swapChain{};

		// TODO: Maybe move instance out of device?

		vk::Queue m_queue{};

		vk::Format m_depthFormat{};

		vk::UniqueCommandPool m_commandPool{};

		vk::PipelineStageFlags m_submitPipelineStages{vk::PipelineStageFlagBits::eColorAttachmentOutput};

		vk::SubmitInfo m_submitInfo{};

		// std::vector<vk::UniqueCommandBuffer> m_drawCommandBuffers{};
		u32 m_currentBuffer{0};

		vk::UniqueDescriptorPool m_descriptorPool{};

		std::vector<vk::UniqueShaderModule> m_shaderModules{};

		vk::UniquePipelineCache m_pipelineCache{};

		struct
		{
			vk::UniqueImage image{};
			vk::UniqueDeviceMemory mem{};
			vk::UniqueImageView view{};
		} m_depthStencil{};

		struct FrameData
		{
			vk::UniqueCommandBuffer cmd{};

			vk::UniqueSemaphore presentCompleteSemaphore{};
			vk::UniqueSemaphore renderCompleteSemaphore{};
			vk::UniqueFence renderCompleteFence{};
		};

		static constexpr u32 m_renderAhead{2};
		std::array<FrameData, m_renderAhead> m_frames{};

		u32 m_frameIndex{0};

		// std::vector<vk::UniqueFence> m_waitFences{};

		bool m_prepared{false};
		bool m_resized{false};
		bool viewUpdated{false};

		mim::vec2i m_framebufferExtent{};

		Logger m_logger{"graphics"};
	};
} // namespace gen