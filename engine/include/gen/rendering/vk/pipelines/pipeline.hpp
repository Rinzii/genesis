// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/rendering/vk/commands/commandBuffer.hpp"
#include "gen/rendering/vk/pipelines/shader.hpp"

namespace gen
{

	class Pipeline
	{
	public:
		Pipeline()			= default;
		virtual ~Pipeline() = default;

		void bindPipeline(const vk::CommandBuffer & commandBuffer) const { commandBuffer.bindPipeline(getPipelineBindPoint(), getPipeline()); }

		virtual const Shader getShader() const								 = 0;
		virtual bool isPushDescriptors() const								 = 0;
		virtual const vk::DescriptorSetLayout getDescriptorSetLayout() const = 0;
		virtual const vk::DescriptorPool getDescriptorPool() const			 = 0;
		virtual const vk::Pipeline getPipeline() const						 = 0;
		virtual const vk::PipelineLayout getPipelineLayout() const			 = 0;
		virtual const vk::PipelineBindPoint getPipelineBindPoint() const	 = 0;
	};
} // namespace gen