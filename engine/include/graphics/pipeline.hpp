// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

#include "core.hpp"

namespace gen
{

	class GraphicsPipeline
	{
	public:
		GraphicsPipeline();
		~GraphicsPipeline();

	private:
		static std::vector<char> readFile(const std::string & filePath);

		void createGraphicsPipeline();
		void destroyGraphicsPipeline();

		void createInstance(std::string const & appName, std::string const & engineName, std::vector<std::string> const & layers,
							std::vector<std::string> const & extensions, u32 const & appVersion);

		void createDebugMessenger();

		vk::Instance m_instance;
		vk::DebugUtilsMessengerEXT m_debugMessenger;
	};

} // namespace gen
