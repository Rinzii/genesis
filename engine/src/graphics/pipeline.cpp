// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "core.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/vkHelpers.hpp"
#include "log.hpp"

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

	    if (!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filePath);
        }

	    std::size_t fileSize = static_cast<size_t>(file.tellg());

		return std::vector<char>();
    }

	void GraphicsPipeline::createGraphicsPipeline() {

    }



	} // namespace gen
