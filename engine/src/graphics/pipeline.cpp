// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "core.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/vkHelpers.hpp"
#include "logger/log.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

namespace gen {
    GraphicsPipeline::GraphicsPipeline(GraphicsDevice &device, const PipelineConfigInfo& configInfo)
	    : m_device(device), m_configInfo(configInfo)
	{
        createGraphicsPipeline();
    }

	GraphicsPipeline::~GraphicsPipeline()
	{
		destroyGraphicsPipeline();
	}

    std::vector<char> GraphicsPipeline::readFile(const std::string & filePath)
	{
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	    if (!file.is_open())
		{
			gen::logger::error("graphics", "failed to open file: " + filePath);
            throw std::runtime_error("failed to open file: " + filePath);
        }

	    std::streamsize const fileSize = file.tellg();

		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
    }

	void GraphicsPipeline::createGraphicsPipeline()
	{
        auto vertShaderCode = readFile("shaders/vert.spv");
		auto fragShaderCode = readFile("shaders/frag.spv");

		gen::logger::debug("graphics", "vertShaderCode size: " + std::to_string(vertShaderCode.size()));
		gen::logger::debug("graphics", "fragShaderCode size: " + std::to_string(fragShaderCode.size()));
    }

	void GraphicsPipeline::destroyGraphicsPipeline()
	{

	}
	void GraphicsPipeline::createShaderModule(const std::vector<char> & code, vk::ShaderModule * shaderModule)
	{
		m_vertShaderModule = vk::util::createShaderModule(m_device.getDevice(), vk::ShaderStageFlagBits::eVertex, code.data());
	}

	PipelineConfigInfo GraphicsPipeline::defaultPipelineConfigInfo(u32 width, u32 height)
    {
        PipelineConfigInfo configInfo{};
        return configInfo;
    }

} // namespace gen
