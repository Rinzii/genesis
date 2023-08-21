// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

#include "core.hpp"
#include "logger/log.hpp"

#include "device.hpp"

namespace gen
{
	struct PipelineConfigInfo
	{
		vk::Viewport viewport{};
		vk::Rect2D scissor{};
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		vk::PipelineTessellationStateCreateInfo tessellationInfo{};
		vk::PipelineViewportStateCreateInfo viewportInfo{};
		vk::PipelineRasterizationStateCreateInfo rasterizationInfo{};
		vk::PipelineMultisampleStateCreateInfo multisampleInfo{};
		vk::PipelineDepthStencilStateCreateInfo depthStencilInfo{};
		vk::PipelineColorBlendStateCreateInfo colorBlendInfo{};
		vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
		uint32_t subpass{};
	};

	class GraphicsPipeline
	{
	public:
		GraphicsPipeline(GraphicsDevice & device, const PipelineConfigInfo & configInfo);
		~GraphicsPipeline();

		GraphicsPipeline(const GraphicsPipeline &)			   = delete;
		GraphicsPipeline & operator=(const GraphicsPipeline &) = delete;

		static PipelineConfigInfo defaultPipelineConfigInfo(mim::vec2i extent);

	private:
		void createRenderPass();

		static std::vector<char> readFile(const std::string & filePath);

		void createGraphicsPipeline();

		void createShaderModule(const std::vector<char> & code);

		// NOLINTNEXTLINE The assumption is that the device will outlive the pipeline at all times since a device fundamentally needs a pipeline to exist
		GraphicsDevice & m_device;
		vk::UniqueShaderModule m_vertShaderModule;
		vk::UniqueShaderModule m_fragShaderModule;

		vk::UniqueRenderPass m_renderPass;
		vk::UniquePipelineLayout m_pipelineLayout;
		vk::UniquePipeline m_graphicsPipeline;

		const PipelineConfigInfo m_configInfo;

		vk::UniqueInstance m_instance;
		vk::DebugUtilsMessengerEXT m_debugMessenger;

		Logger m_logger{"graphics"};
	};

} // namespace gen