// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "core.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/vkHelpers.hpp"
#include "logger/log.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

namespace gen
{
	GraphicsPipeline::GraphicsPipeline(GraphicsDevice & device, const PipelineConfigInfo & configInfo)
		: m_device{device}, m_configInfo{configInfo}
	{
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
	}

	PipelineConfigInfo GraphicsPipeline::defaultPipelineConfigInfo(const mim::vec2i extent)
	{
		PipelineConfigInfo configInfo{};
		return configInfo;
	}

	std::vector<char> GraphicsPipeline::readFile(const std::string & filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
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
	}

	void GraphicsPipeline::createShaderModule(const std::vector<char> & code, vk::ShaderModule * shaderModule)
	{
	}


} // namespace gen