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
		GraphicsPipeline(GraphicsPipeline &&)				   = delete;
		GraphicsPipeline & operator=(GraphicsPipeline &&)	   = delete;

		static PipelineConfigInfo defaultPipelineConfigInfo(const mim::vec2i& extent);

	private:
		static std::vector<char> readFile(const std::string & filePath);

		void createGraphicsPipeline();

		// NOLINTNEXTLINE The assumption is that the device will outlive the pipeline at all times since a device fundamentally needs a pipeline to exist
		GraphicsDevice & m_device;
		vk::UniqueShaderModule m_vertShaderModule{};
		vk::UniqueShaderModule m_fragShaderModule{};

		vk::UniquePipelineLayout m_pipelineLayout{};
		vk::UniquePipeline m_graphicsPipeline{};

		const PipelineConfigInfo m_configInfo{};


		Logger m_logger{"graphics"};
	};

} // namespace gen