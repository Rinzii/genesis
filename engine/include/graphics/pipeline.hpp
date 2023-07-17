// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "core.hpp"

namespace gen {

    class GraphicsPipeline {
    public:
		GraphicsPipeline();
		~GraphicsPipeline();

    private:
        static std::vector<char> readFile(const std::string& filePath);

        void createGraphicsPipeline();



		vk::Instance m_instance;
		vk::DebugUtilsMessengerEXT m_debugMessenger;
    };

} // namespace gen
