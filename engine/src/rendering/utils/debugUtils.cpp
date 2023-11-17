// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/utils/debugUtils.hpp"

namespace
{
#if !defined(GEN_NDEBUG) || defined(GEN_FORCE_VALIDATION_LAYERS)
	constexpr bool debugEnabled = true;
#else
	constexpr bool debugEnabled = false;
#endif
} // namespace

namespace vk::debug
{

	void cmdBeginLabel(vk::CommandBuffer commandBuffer, const char * labelName, const std::vector<float> & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsLabelEXT label{};
		label.pLabelName = labelName;
		label.color[0]	 = color[0];
		label.color[1]	 = color[1];
		label.color[2]	 = color[2];
		label.color[3]	 = color[3];
		commandBuffer.beginDebugUtilsLabelEXT(label);
	}

	void cmdBeginLabel(vk::CommandBuffer commandBuffer, const char * labelName, const mim::vec4f & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsLabelEXT label{};
		label.pLabelName = labelName;
		label.color[0]	 = color[0];
		label.color[1]	 = color[1];
		label.color[2]	 = color[2];
		label.color[3]	 = color[3];
		commandBuffer.beginDebugUtilsLabelEXT(label);
	}

	void cmdInsertLabel(vk::CommandBuffer commandBuffer, const char * labelName, const std::vector<float> & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsLabelEXT label{};
		label.pLabelName = labelName;
		label.color[0]	 = color[0];
		label.color[1]	 = color[1];
		label.color[2]	 = color[2];
		label.color[3]	 = color[3];
		commandBuffer.insertDebugUtilsLabelEXT(label);
	}

	void cmdInsertLabel(vk::CommandBuffer commandBuffer, const char * labelName, const mim::vec4f & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsLabelEXT label{};
		label.pLabelName = labelName;
		label.color[0]	 = color[0];
		label.color[1]	 = color[1];
		label.color[2]	 = color[2];
		label.color[3]	 = color[3];
		commandBuffer.insertDebugUtilsLabelEXT(label);
	}

	void cmdEndLabel(vk::CommandBuffer commandBuffer)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		commandBuffer.endDebugUtilsLabelEXT();
	}

	void queueBeginLabel(vk::Queue queue, const char * labelName, const std::vector<float> & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsLabelEXT label{};
		label.pLabelName = labelName;
		label.color[0]	 = color[0];
		label.color[1]	 = color[1];
		label.color[2]	 = color[2];
		label.color[3]	 = color[3];
		queue.beginDebugUtilsLabelEXT(label);
	}

	void queueBeginLabel(vk::Queue queue, const char * labelName, const mim::vec4f & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsLabelEXT label{};
		label.pLabelName = labelName;
		label.color[0]	 = color[0];
		label.color[1]	 = color[1];
		label.color[2]	 = color[2];
		label.color[3]	 = color[3];
		queue.beginDebugUtilsLabelEXT(label);
	}

	void queueInsertLabel(vk::Queue queue, const char * labelName, const std::vector<float> & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsLabelEXT label{};
		label.pLabelName = labelName;
		label.color[0]	 = color[0];
		label.color[1]	 = color[1];
		label.color[2]	 = color[2];
		label.color[3]	 = color[3];
		queue.insertDebugUtilsLabelEXT(label);
	}

	void queueInsertLabel(vk::Queue queue, const char * labelName, const mim::vec4f & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsLabelEXT label{};
		label.pLabelName = labelName;
		label.color[0]	 = color[0];
		label.color[1]	 = color[1];
		label.color[2]	 = color[2];
		label.color[3]	 = color[3];
		queue.insertDebugUtilsLabelEXT(label);
	}

	void queueEndLabel(vk::Queue queue)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		queue.endDebugUtilsLabelEXT();
	}

	void setObjectName(vk::Device & handle, vk::ObjectType objectType, gen::u64 objectHandle, const char * name)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsObjectNameInfoEXT nameInfo{};
		nameInfo.objectType	  = objectType;
		nameInfo.objectHandle = objectHandle;
		nameInfo.pObjectName  = name;
		handle.setDebugUtilsObjectNameEXT(nameInfo);
	}
} // namespace vk::debug