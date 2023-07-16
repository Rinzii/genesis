// Copyright Ian Pike. All Rights Reserved.

#include "graphics/pipeline.hpp"

#include <fstream>
#include <stdexcept>

namespace gen {
Pipeline::Pipeline(std::string const& vertFilePath, std::string const& fragFilePath) { createGraphicsPipeline(vertFilePath, fragFilePath); }

std::vector<char> Pipeline::readFile(std::string const& filePath) {
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) { throw std::runtime_error("failed to open file: " + filePath); }

	std::size_t fileSize = static_cast<size_t>(file.tellg());

	return std::vector<char>();
}

void Pipeline::createGraphicsPipeline(std::string const& vertFilePath, std::string const& fragFilePath) {}

} // namespace gen
