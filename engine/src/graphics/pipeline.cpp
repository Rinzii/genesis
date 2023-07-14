// Copyright Ian Pike. All Rights Reserved.

#include "graphics/pipeline.hpp"

#include <fstream>
#include <stdexcept>

namespace gen {
    Pipeline::Pipeline(const std::string& vertFilePath, const std::string& fragFilePath) {
        createGraphicsPipeline(vertFilePath, fragFilePath);
    }

    std::vector<char> Pipeline::readFile(const std::string & filePath) {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	    if (!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filePath);
        }

	    std::size_t fileSize = static_cast<size_t>(file.tellg());

		return std::vector<char>();
    }

	void Pipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string &fragFilePath) {

    }

} // namespace gen
