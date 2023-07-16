// Copyright Ian Pike. All Rights Reserved.

#pragma once

#include <string>
#include <vector>

namespace gen {

class Pipeline {
public:
	Pipeline(std::string const& vertFilePath, std::string const& fragFilePath);

private:
	static std::vector<char> readFile(std::string const& filePath);

	void createGraphicsPipeline(std::string const& vertFilePath, std::string const& fragFilePath);
};

} // namespace gen
