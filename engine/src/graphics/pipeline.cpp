// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/pipeline.hpp"
#include "graphics/vkHelpers.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace gen
{
	GraphicsPipeline::GraphicsPipeline(GraphicsDevice & device, const PipelineConfigInfo & configInfo)
		: m_device{device}, m_configInfo{configInfo}
	{
		createGraphicsPipeline();
		m_logger.info("Graphics pipeline constructed");
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		m_logger.info("Graphics pipeline destructed");
	}

	PipelineConfigInfo GraphicsPipeline::defaultPipelineConfigInfo(const mim::vec2i& extent)
	{
		PipelineConfigInfo configInfo{};

		configInfo.viewport.x		 = 0.0F;
		configInfo.viewport.y		 = 0.0F;
		configInfo.viewport.width	 = static_cast<float>(extent.x);
		configInfo.viewport.height	 = static_cast<float>(extent.y);
		configInfo.viewport.minDepth = 0.0F;
		configInfo.viewport.maxDepth = 1.0F;
		configInfo.scissor.offset	 = vk::Offset2D{0, 0};
		configInfo.scissor.extent	 = vk::Extent2D{static_cast<u32>(extent.x), static_cast<u32>(extent.y)};

		configInfo.inputAssemblyInfo.flags					= vk::PipelineInputAssemblyStateCreateFlags{};
		configInfo.inputAssemblyInfo.topology				= vk::PrimitiveTopology::eTriangleList;
		configInfo.inputAssemblyInfo.primitiveRestartEnable = false; // NOLINT(readability-implicit-bool-conversion)

		configInfo.viewportInfo.flags		  = vk::PipelineViewportStateCreateFlags{};
		configInfo.viewportInfo.sType		  = vk::StructureType::ePipelineViewportStateCreateInfo;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports	  = &configInfo.viewport;
		configInfo.viewportInfo.scissorCount  = 1;
		configInfo.viewportInfo.pScissors	  = &configInfo.scissor;

		configInfo.rasterizationInfo.flags					 = vk::PipelineRasterizationStateCreateFlags{};
		configInfo.rasterizationInfo.sType					 = vk::StructureType::ePipelineRasterizationStateCreateInfo;
		configInfo.rasterizationInfo.depthClampEnable		 = false; // NOLINT(readability-implicit-bool-conversion)
		configInfo.rasterizationInfo.rasterizerDiscardEnable = false; // NOLINT(readability-implicit-bool-conversion)
		configInfo.rasterizationInfo.polygonMode			 = vk::PolygonMode::eFill;
		configInfo.rasterizationInfo.lineWidth				 = 1.0F;
		configInfo.rasterizationInfo.cullMode				 = vk::CullModeFlagBits::eNone; // TODO: Switch this to eBack once done testing.
		configInfo.rasterizationInfo.frontFace				 = vk::FrontFace::eClockwise;
		configInfo.rasterizationInfo.depthBiasEnable		 = false; // NOLINT(readability-implicit-bool-conversion)
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0F; // Optional
		configInfo.rasterizationInfo.depthBiasClamp			 = 0.0F; // Optional
		configInfo.rasterizationInfo.depthBiasSlopeFactor	 = 0.0F; // Optional

		// Multi-sampling is not currently enabled. This will be enabled later.
		configInfo.multisampleInfo.flags				 = vk::PipelineMultisampleStateCreateFlags{};
		configInfo.multisampleInfo.sType				 = vk::StructureType::ePipelineMultisampleStateCreateInfo;
		configInfo.multisampleInfo.rasterizationSamples	 = vk::SampleCountFlagBits::e1;
		configInfo.multisampleInfo.sampleShadingEnable	 = false; // NOLINT(readability-implicit-bool-conversion)
		configInfo.multisampleInfo.minSampleShading		 = 1.0F;	// Optional
		configInfo.multisampleInfo.pSampleMask			 = nullptr; // Optional
		configInfo.multisampleInfo.alphaToCoverageEnable = false;	// Optional // NOLINT(readability-implicit-bool-conversion)
		configInfo.multisampleInfo.alphaToOneEnable		 = false;	// Optional // NOLINT(readability-implicit-bool-conversion)

		// implement depth and stencil later

		configInfo.colorBlendAttachment.colorWriteMask =
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		// this implements alpha blending
		configInfo.colorBlendAttachment.blendEnable			= true; // NOLINT(readability-implicit-bool-conversion)
		configInfo.colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;		  // Optional
		configInfo.colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha; // Optional
		configInfo.colorBlendAttachment.colorBlendOp		= vk::BlendOp::eAdd;				  // Optional
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;			  // Optional
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;			  // Optional
		configInfo.colorBlendAttachment.alphaBlendOp		= vk::BlendOp::eAdd;				  // Optional

		configInfo.colorBlendInfo.flags				= vk::PipelineColorBlendStateCreateFlags{};
		configInfo.colorBlendInfo.sType				= vk::StructureType::ePipelineColorBlendStateCreateInfo;
		configInfo.colorBlendInfo.logicOpEnable		= true; // NOLINT(readability-implicit-bool-conversion)
		configInfo.colorBlendInfo.logicOp			= vk::LogicOp::eCopy; // Optional
		configInfo.colorBlendInfo.attachmentCount	= 1;
		configInfo.colorBlendInfo.pAttachments		= &configInfo.colorBlendAttachment;
		configInfo.colorBlendInfo.blendConstants[0] = 0.0F; // Optional
		configInfo.colorBlendInfo.blendConstants[1] = 0.0F; // Optional
		configInfo.colorBlendInfo.blendConstants[2] = 0.0F; // Optional
		configInfo.colorBlendInfo.blendConstants[3] = 0.0F; // Optional

		configInfo.subpass = 0;

		return configInfo;
	}

	std::vector<char> GraphicsPipeline::readFile(const std::string & filePath)
	{
		// This function is temporary for testing purposes. Will replace later.

		const Logger logger{"graphics"};

		std::string const currentDir = std::filesystem::current_path().string();
		std::string path	   = currentDir + "/" + filePath;

		logger.info("Attempting to open file: {}", path);

		std::ifstream file(path, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			logger.error("Failed to open file: {}", filePath);
			throw std::runtime_error("failed to open file: " + filePath);
		}

		std::streamsize const fileSize = file.tellg();

		std::vector<char> buffer(static_cast<std::size_t>(fileSize));

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	void GraphicsPipeline::createGraphicsPipeline()
	{
		// This will be removed later and replaced with a better system.
		// Just here so we can eventually render a triangle.
		auto vertShaderCode = readFile("bin/simpleVS.spv");
		auto fragShaderCode = readFile("bin/simplePS.spv");

		m_vertShaderModule = vk::util::createShaderModule(m_device.getDevice(), vertShaderCode);
		m_fragShaderModule = vk::util::createShaderModule(m_device.getDevice(), fragShaderCode);

		vk::PipelineShaderStageCreateInfo const vertShaderStageInfo{{}, vk::ShaderStageFlagBits::eVertex, m_vertShaderModule.get(), "main"};
		vk::PipelineShaderStageCreateInfo const fragShaderStageInfo{{}, vk::ShaderStageFlagBits::eFragment, m_fragShaderModule.get(), "main"};

		// vk::PipelineVertexInputStateCreateInfo vertexInputInfo{{}, 0, nullptr, 0, nullptr};

		std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {vertShaderStageInfo, fragShaderStageInfo};

		std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

		vk::PipelineDynamicStateCreateInfo const dynamicStateCreateInfo{{}, static_cast<uint32_t>(dynamicStates.size()), dynamicStates.data()};

		vk::PipelineVertexInputStateCreateInfo const vertexInputInfo{{}, 0, nullptr, 0, nullptr};

		m_pipelineLayout = m_device.getUniqueDevice()->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo{{}, 0, nullptr, 0, nullptr});

		vk::GraphicsPipelineCreateInfo const pipelineInfo{
			{},
			2,
			shaderStages.data(),
			&vertexInputInfo,
			&m_configInfo.inputAssemblyInfo,
			&m_configInfo.tessellationInfo,
			&m_configInfo.viewportInfo,
			&m_configInfo.rasterizationInfo,
			&m_configInfo.multisampleInfo,
			&m_configInfo.depthStencilInfo,
			&m_configInfo.colorBlendInfo,
			&dynamicStateCreateInfo,
			m_pipelineLayout.get(),
			nullptr, // render pass is no longer needed with dynamic rendering
			m_configInfo.subpass,
			nullptr, // optional
			-1};	 // optional

		m_graphicsPipeline = m_device.getUniqueDevice()->createGraphicsPipelineUnique(nullptr, pipelineInfo).value;
	}


} // namespace gen