// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/command.hpp"

namespace gen
{
	CommandBufferPool::CommandBufferPool(const Device & device)
	{
		createCommandPoolAndBuffer(device);
	}


	void CommandBufferPool::createCommandPoolAndBuffer(const Device & device)
	{
		// TODO: Does a command buffer and a command pool belong to a device? Or should to go elsewhere?
		auto commandPoolCreateInfo			   = vk::CommandPoolCreateInfo{};
		commandPoolCreateInfo.queueFamilyIndex = device.getGpu().queueFamily;
		commandPoolCreateInfo.flags			   = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

		m_commandPool = device.getDevice().createCommandPoolUnique(commandPoolCreateInfo);

		auto commandBufferAllocateInfo				 = vk::CommandBufferAllocateInfo{};
		commandBufferAllocateInfo.commandPool		 = m_commandPool.get();
		commandBufferAllocateInfo.commandBufferCount = 1;

		m_commandBuffer = device.getDevice().allocateCommandBuffers(commandBufferAllocateInfo).front();
	}
} // namespace gen