// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/vk/buffers/buffer.hpp"

#include "gen/rendering/vk/devices/logicalDevice.hpp"

namespace gen
{
	gen::Buffer::Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, const void * data)
	{
		auto graphicsFamily = Device::self().getGraphicsQueue();
		// NOTE: May add support for multi queue family later
		std::array queueFamilyIndices = {graphicsFamily};

		vk::BufferCreateInfo bufferInfo	 = {};
		bufferInfo.size					 = size;
		bufferInfo.usage				 = usage;
		bufferInfo.sharingMode			 = vk::SharingMode::eExclusive;
		bufferInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
		bufferInfo.pQueueFamilyIndices	 = queueFamilyIndices;
	}

	Buffer::~Buffer()
	{
	}

} // namespace gen