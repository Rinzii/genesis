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
    GraphicsPipeline::GraphicsPipeline() {
        createGraphicsPipeline();
    }

	GraphicsPipeline::~GraphicsPipeline() {
		destroyGraphicsPipeline();
	}

    std::vector<char> GraphicsPipeline::readFile(const std::string & filePath) {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	    if (!file.is_open())
		{
			gen::logger::error("graphics", "failed to open file: " + filePath);
            throw std::runtime_error("failed to open file: " + filePath);
        }

	    std::size_t const fileSize = static_cast<std::size_t>(file.tellg());

		return std::vector<char>();
    }

	void GraphicsPipeline::createGraphicsPipeline() {

    }

	void GraphicsPipeline::destroyGraphicsPipeline()
	{

	}



	} // namespace gen
