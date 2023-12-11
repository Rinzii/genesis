// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vulkan/vulkan.hpp>

#include <mim/vec4.hpp>

#include "gen/system/types.hpp"

#if defined( _MSC_VER ) && !defined( _WIN64 )
	#  define NON_DISPATCHABLE_UINT64_CAST( type, x ) static_cast<type>( x )
#else
	#  define NON_DISPATCHABLE_UINT64_CAST( type, x ) reinterpret_cast<uint64_t>( static_cast<type>( x ) )
#endif

namespace vk::debug
{
	void cmdBeginLabel(vk::CommandBuffer & commandBuffer, const char * labelName, std::vector<float> const & color);
	void cmdBeginLabel(vk::CommandBuffer & commandBuffer, const char * labelName, mim::vec4f const & color);
	void cmdBeginRegion(vk::CommandBuffer & commandBuffer, const char * labelName, std::vector<float> const & color);
	void cmdBeginRegion(vk::CommandBuffer & commandBuffer, const char * labelName, mim::vec4f const & color);

	void cmdInsertLabel(vk::CommandBuffer & commandBuffer, const char * labelName, std::vector<float> const & color);
	void cmdInsertLabel(vk::CommandBuffer & commandBuffer, const char * labelName, mim::vec4f const & color);
	void cmdInsertRegion(vk::CommandBuffer & commandBuffer, const char * labelName, std::vector<float> const & color);
	void cmdInsertRegion(vk::CommandBuffer & commandBuffer, const char * labelName, mim::vec4f const & color);

	void cmdEndLabel(vk::CommandBuffer & commandBuffer);
	void cmdEndRegion(vk::CommandBuffer & commandBuffer);

	void queueBeginLabel(vk::Queue & queue, const char * labelName, std::vector<float> const & color);
	void queueBeginLabel(vk::Queue & queue, const char * labelName, mim::vec4f const & color);

	void queueInsertLabel(vk::Queue & queue, const char * labelName, std::vector<float> const & color);
	void queueInsertLabel(vk::Queue & queue, const char * labelName, mim::vec4f const & color);

	void queueEndLabel(vk::Queue & queue);

	void setObjectName(vk::Device & handle, vk::ObjectType objectType, gen::u64 objectHandle, const char * name);

	void setObjectTag(vk::Device & handle, vk::ObjectType objectType, gen::u64 objectHandle, gen::u64 name, gen::u64 tagSize, const void * tag);

} // namespace vk::debug