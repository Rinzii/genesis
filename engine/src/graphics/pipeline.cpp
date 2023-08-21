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
	GraphicsPipeline::GraphicsPipeline(GraphicsDevice & device, const PipelineConfigInfo & configInfo) : m_device{device}, m_configInfo{configInfo}
	{
		createRenderPass();
		createGraphicsPipeline();
		m_logger.info("Graphics pipeline constructed");
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		m_logger.info("Graphics pipeline destructed");
	}

	PipelineConfigInfo GraphicsPipeline::defaultPipelineConfigInfo(const mim::vec2i extent)
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
		configInfo.inputAssemblyInfo.primitiveRestartEnable = false;

		configInfo.viewportInfo.flags		  = vk::PipelineViewportStateCreateFlags{};
		configInfo.viewportInfo.sType		  = vk::StructureType::ePipelineViewportStateCreateInfo;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports	  = &configInfo.viewport;
		configInfo.viewportInfo.scissorCount  = 1;
		configInfo.viewportInfo.pScissors	  = &configInfo.scissor;

		configInfo.rasterizationInfo.flags					 = vk::PipelineRasterizationStateCreateFlags{};
		configInfo.rasterizationInfo.sType					 = vk::StructureType::ePipelineRasterizationStateCreateInfo;
		configInfo.rasterizationInfo.depthClampEnable		 = false;
		configInfo.rasterizationInfo.rasterizerDiscardEnable = false;
		configInfo.rasterizationInfo.polygonMode			 = vk::PolygonMode::eFill;
		configInfo.rasterizationInfo.lineWidth				 = 1.0F;
		configInfo.rasterizationInfo.cullMode				 = vk::CullModeFlagBits::eNone; // TODO: Switch this to eBack once done testing.
		configInfo.rasterizationInfo.frontFace				 = vk::FrontFace::eClockwise;
		configInfo.rasterizationInfo.depthBiasEnable		 = false;
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0F; // Optional
		configInfo.rasterizationInfo.depthBiasClamp			 = 0.0F; // Optional
		configInfo.rasterizationInfo.depthBiasSlopeFactor	 = 0.0F; // Optional

		// Multi-sampling is not currently enabled. This will be enabled later.
		configInfo.multisampleInfo.flags				 = vk::PipelineMultisampleStateCreateFlags{};
		configInfo.multisampleInfo.sType				 = vk::StructureType::ePipelineMultisampleStateCreateInfo;
		configInfo.multisampleInfo.rasterizationSamples	 = vk::SampleCountFlagBits::e1;
		configInfo.multisampleInfo.sampleShadingEnable	 = false;
		configInfo.multisampleInfo.minSampleShading		 = 1.0F;	// Optional
		configInfo.multisampleInfo.pSampleMask			 = nullptr; // Optional
		configInfo.multisampleInfo.alphaToCoverageEnable = false;	// Optional
		configInfo.multisampleInfo.alphaToOneEnable		 = false;	// Optional

		// implement depth and stencil later

		configInfo.colorBlendAttachment.colorWriteMask =
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		// this implements alpha blending
		configInfo.colorBlendAttachment.blendEnable			= true;
		configInfo.colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;		  // Optional
		configInfo.colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha; // Optional
		configInfo.colorBlendAttachment.colorBlendOp		= vk::BlendOp::eAdd;				  // Optional
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;			  // Optional
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;			  // Optional
		configInfo.colorBlendAttachment.alphaBlendOp		= vk::BlendOp::eAdd;				  // Optional

		configInfo.colorBlendInfo.flags				= vk::PipelineColorBlendStateCreateFlags{};
		configInfo.colorBlendInfo.sType				= vk::StructureType::ePipelineColorBlendStateCreateInfo;
		configInfo.colorBlendInfo.logicOpEnable		= true;
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

		std::string currentDir = std::filesystem::current_path().string();
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

	void GraphicsPipeline::createRenderPass()
	{
		vk::AttachmentDescription colorAttachment{
			{},
			m_device.getSwapChainInfo().imageFormat,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,  // Clear framebuffer to black before drawing a new frame.
			vk::AttachmentStoreOp::eStore, // Store the framebuffer to memory after drawing a new frame.
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::ePresentSrcKHR};

		vk::AttachmentReference colorAttachmentRef{0, vk::ImageLayout::eColorAttachmentOptimal};

		// for the time being we are only doing a single subpass
		vk::SubpassDescription subpass{{}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef, nullptr, nullptr, 0, nullptr};

		vk::SubpassDependency dependency{
			VK_SUBPASS_EXTERNAL,
			0,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			{},
			vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite};

		vk::RenderPassCreateInfo renderPassInfo{{}, 1, &colorAttachment, 1, &subpass, 1, &dependency};

		m_renderPass = m_device.getUniqueDevice()->createRenderPassUnique(renderPassInfo);
	}

	void GraphicsPipeline::createGraphicsPipeline()
	{
		// This will be removed later.
		auto vertShaderCode = readFile("bin/simplePS.spv");
		auto fragShaderCode = readFile("bin/simpleVS.spv");

		m_vertShaderModule = vk::util::createShaderModule(m_device.getDevice(), vertShaderCode);
		m_fragShaderModule = vk::util::createShaderModule(m_device.getDevice(), fragShaderCode);

		vk::PipelineShaderStageCreateInfo vertShaderStageInfo{{}, vk::ShaderStageFlagBits::eVertex, m_vertShaderModule.get(), "main"};
		vk::PipelineShaderStageCreateInfo fragShaderStageInfo{{}, vk::ShaderStageFlagBits::eFragment, m_fragShaderModule.get(), "main"};

		// vk::PipelineVertexInputStateCreateInfo vertexInputInfo{{}, 0, nullptr, 0, nullptr};

		std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {vertShaderStageInfo, fragShaderStageInfo};

		std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

		vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo{{}, static_cast<uint32_t>(dynamicStates.size()), dynamicStates.data()};

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo{{}, 0, nullptr, 0, nullptr};

		m_pipelineLayout = m_device.getUniqueDevice()->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo{{}, 0, nullptr, 0, nullptr});

		vk::GraphicsPipelineCreateInfo pipelineInfo{
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
			m_renderPass.get(),
			m_configInfo.subpass,
			nullptr, // optional
			-1};	 // optional

		m_graphicsPipeline = m_device.getUniqueDevice()->createGraphicsPipelineUnique(nullptr, pipelineInfo).value;
	}

	void GraphicsPipeline::createShaderModule(const std::vector<char> & code)
	{
	}

} // namespace gen