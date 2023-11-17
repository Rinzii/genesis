// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/frame.hpp"
#include "gen/rendering/device.hpp"
#include "gen/rendering/utils/vkHelpers.hpp"

namespace gen
{
	Frame::Frame(FrameSwapchainData && swapchainData, vk::CommandPoolCreateFlagBits resetFlags) : m_swapchainData{std::move(swapchainData)}
	{
		vk::CommandPoolCreateInfo poolInfo{};
		poolInfo.queueFamilyIndex = Device::self().getQueueFamilyIndex();
		poolInfo.flags			  = resetFlags;
	}

	Frame::Frame(std::vector<vk::Image> && image, vk::CommandPoolCreateFlagBits commandPoolCreateBits)
		: Frame(FrameSwapchainData{std::move(image), {}}, commandPoolCreateBits)
	{
	}

	Frame::Frame(std::vector<vk::UniqueImageView> && imageView, vk::CommandPoolCreateFlagBits commandPoolCreateBits)
		: Frame(FrameSwapchainData{{}, std::move(imageView)}, commandPoolCreateBits)
	{
	}

	void Frame::reset()
	{
		vk::utils::checkResult(Device::self().getHandle().waitForFences(1, &m_fence, vk::True, std::numeric_limits<u32>::max()));
		vk::utils::checkResult(Device::self().getHandle().resetFences(1, &m_fence));

		Device::self().getHandle().resetCommandPool(m_commandPool.get(), vk::CommandPoolResetFlagBits::eReleaseResources);
	}

	void Frame::updateFrame()
	{
	}

	vk::CommandPool Frame::getCommandPool(u32 queueFamilyIndex, u32 queueIndex)
	{
		return vk::CommandPool();
	}

	vk::DescriptorPool Frame::getDescriptorPool(u32 queueFamilyIndex, std::size_t hash)
	{
		return vk::DescriptorPool();
	}

	vk::DescriptorSet Frame::getDescriptorSet(u32 queueFamilyIndex, std::size_t hash)
	{
		return vk::DescriptorSet();
	}

} // namespace gen