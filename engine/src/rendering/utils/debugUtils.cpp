// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/utils/debugUtils.hpp"

namespace
{

#if (defined(GEN_DEBUG) || defined(GEN_VK_FORCE_VALIDATION_LAYERS)) && !defined(GEN_VK_DISABLE_VALIDATION_LAYERS) &&                                           \
	!(defined(GEN_NDEBUG) && !defined(GEN_VK_FORCE_VALIDATION_LAYERS))
	constexpr bool debugEnabled = true;
#else
	constexpr bool debugEnabled = false;
#endif
} // namespace

namespace vk::debug
{

	void cmdBeginLabel(vk::CommandBuffer & commandBuffer, const char * labelName, const std::vector<float> & color)
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

	void cmdBeginLabel(vk::CommandBuffer & commandBuffer, const char * labelName, const mim::vec4f & color)
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

	void cmdBeginRegion(vk::CommandBuffer & commandBuffer, const char * regionName, std::vector<float> const & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugMarkerMarkerInfoEXT marker{};
		marker.pMarkerName = regionName;
		marker.color[0]	   = color[0];
		marker.color[1]	   = color[1];
		marker.color[2]	   = color[2];
		marker.color[3]	   = color[3];
		commandBuffer.debugMarkerBeginEXT(marker);
	}

	void cmdBeginRegion(vk::CommandBuffer & commandBuffer, const char * regionName, mim::vec4f const & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugMarkerMarkerInfoEXT marker{};
		marker.pMarkerName = regionName;
		marker.color[0]	   = color[0];
		marker.color[1]	   = color[1];
		marker.color[2]	   = color[2];
		marker.color[3]	   = color[3];
		commandBuffer.debugMarkerBeginEXT(marker);
	}

	void cmdInsertLabel(vk::CommandBuffer & commandBuffer, const char * labelName, const std::vector<float> & color)
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

	void cmdInsertLabel(vk::CommandBuffer & commandBuffer, const char * labelName, const mim::vec4f & color)
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

	void cmdInsertRegion(vk::CommandBuffer & commandBuffer, const char * markerName, const std::vector<float> & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugMarkerMarkerInfoEXT marker{};
		marker.pMarkerName = markerName;
		marker.color[0]	   = color[0];
		marker.color[1]	   = color[1];
		marker.color[2]	   = color[2];
		marker.color[3]	   = color[3];
		commandBuffer.debugMarkerInsertEXT(marker);
	}

	void cmdInsertRegion(vk::CommandBuffer & commandBuffer, const char * markerName, const mim::vec4f & color)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugMarkerMarkerInfoEXT marker{};
		marker.pMarkerName = markerName;
		marker.color[0]	   = color[0];
		marker.color[1]	   = color[1];
		marker.color[2]	   = color[2];
		marker.color[3]	   = color[3];
		commandBuffer.debugMarkerInsertEXT(marker);
	}

	void cmdEndLabel(vk::CommandBuffer & commandBuffer)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		commandBuffer.endDebugUtilsLabelEXT();
	}

	void cmdEndRegion(vk::CommandBuffer & commandBuffer)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		commandBuffer.debugMarkerEndEXT();
	}

	void queueBeginLabel(vk::Queue & queue, const char * labelName, const std::vector<float> & color)
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

	void queueBeginLabel(vk::Queue & queue, const char * labelName, const mim::vec4f & color)
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

	void queueInsertLabel(vk::Queue & queue, const char * labelName, const std::vector<float> & color)
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

	void queueInsertLabel(vk::Queue & queue, const char * labelName, const mim::vec4f & color)
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

	void setObjectTag(vk::Device & handle, vk::ObjectType objectType, gen::u64 objectHandle, gen::u64 name, gen::u64 tagSize, const void * tag)
	{
		if (!debugEnabled) // NOLINT
		{
			return;
		}

		vk::DebugUtilsObjectTagInfoEXT tagInfo{};
		tagInfo.objectType	 = objectType;
		tagInfo.objectHandle = objectHandle;
		tagInfo.tagName		 = name;
		tagInfo.tagSize		 = tagSize;
		tagInfo.pTag		 = tag;
		handle.setDebugUtilsObjectTagEXT(tagInfo);
	}
} // namespace vk::debug