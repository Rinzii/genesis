// Copyright Ian Pike. All Rights Reserved.

#pragma once

#include <string>
#include <vector>

namespace gen
{

	class Pipeline
	{
	public:
		Pipeline(const std::string & vertFilePath, const std::string & fragFilePath);

	private:
		static std::vector<char> readFile(const std::string & filePath);

		void createGraphicsPipeline(const std::string & vertFilePath, const std::string & fragFilePath);
	};

} // namespace gen
