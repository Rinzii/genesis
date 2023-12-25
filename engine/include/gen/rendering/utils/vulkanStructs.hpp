// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>

namespace vk::types
{
	struct ImageMemoryBarrier
	{
		vk::PipelineStageFlags srcStageMask = {vk::PipelineStageFlagBits::eBottomOfPipe};
		vk::PipelineStageFlags dstStageMask = {vk::PipelineStageFlagBits::eTopOfPipe};
		vk::AccessFlags srcAccessMask		= {};
		vk::AccessFlags dstAccessMask		= {};
		vk::ImageLayout oldLayout			= {vk::ImageLayout::eUndefined};
		vk::ImageLayout newLayout			= {vk::ImageLayout::eUndefined};
		gen::u32 srcQueueFamilyIndex		= {VK_QUEUE_FAMILY_IGNORED};
		gen::u32 dstQueueFamilyIndex		= {VK_QUEUE_FAMILY_IGNORED};
	};

	struct BufferMemoryBarrier
	{
		vk::PipelineStageFlags srcStageMask = {vk::PipelineStageFlagBits::eBottomOfPipe};
		vk::PipelineStageFlags dstStageMask = {vk::PipelineStageFlagBits::eTopOfPipe};
		vk::AccessFlags srcAccessMask		= {};
		vk::AccessFlags dstAccessMask		= {};
	};

	struct LoadStoreInfo
	{
		vk::AttachmentLoadOp loadOp	  = {vk::AttachmentLoadOp::eClear};
		vk::AttachmentStoreOp storeOp = {vk::AttachmentStoreOp::eStore};
	};
} // namespace vk::types