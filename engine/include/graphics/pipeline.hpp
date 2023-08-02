// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "core.hpp"

#include "device.hpp"

namespace gen
{
	struct PipelineConfigInfo
	{

	};


    class GraphicsPipeline {
    public:
		GraphicsPipeline(GraphicsDevice &device, const PipelineConfigInfo& configInfo);
		~GraphicsPipeline();

		GraphicsPipeline(const GraphicsPipeline&) = delete;
		GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

		static PipelineConfigInfo defaultPipelineConfigInfo(u32 width, u32 height);

    private:
        static std::vector<char> readFile(const std::string& filePath);

        void createGraphicsPipeline();

		void createShaderModule(const std::vector<char> & code, vk::ShaderModule * shaderModule);



		void destroyGraphicsPipeline();


		// NOLINTNEXTLINE The assumption with this is that the device will outlive the pipeline at all times since a pipeline fundamentally needs a pipeline to exist
		GraphicsDevice& m_device;
		vk::Pipeline m_graphicsPipeline;
		vk::ShaderModule m_vertShaderModule;
		vk::ShaderModule m_fragShaderModule;

		const PipelineConfigInfo m_configInfo;

		vk::Instance m_instance;
		vk::DebugUtilsMessengerEXT m_debugMessenger;
    };

} // namespace gen
