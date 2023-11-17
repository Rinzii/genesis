// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vulkan/vulkan.hpp>

#include <mim/vec4.hpp>

#include "gen/system/types.hpp"

namespace vk::debug
{
	void cmdBeginLabel(vk::CommandBuffer commandBuffer, const char * labelName, std::vector<float> const & color);
	void cmdBeginLabel(vk::CommandBuffer commandBuffer, const char * labelName, mim::vec4f const & color);

	void cmdInsertLabel(vk::CommandBuffer commandBuffer, const char * labelName, std::vector<float> const & color);
	void cmdInsertLabel(vk::CommandBuffer commandBuffer, const char * labelName, mim::vec4f const & color);

	void cmdEndLabel(vk::CommandBuffer commandBuffer);

	void queueBeginLabel(vk::Queue queue, const char * labelName, std::vector<float> const & color);
	void queueBeginLabel(vk::Queue queue, const char * labelName, mim::vec4f const & color);

	void queueInsertLabel(vk::Queue queue, const char * labelName, std::vector<float> const & color);
	void queueInsertLabel(vk::Queue queue, const char * labelName, mim::vec4f const & color);

	void queueEndLabel(vk::Queue queue);

	void setObjectName(vk::Device & handle, vk::ObjectType objectType, gen::u64 objectHandle, const char * name);

} // namespace vk::debug