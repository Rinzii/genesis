// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/vk/queue.hpp"

#include "gen/rendering/vk/devices/logicalDevice.hpp"

namespace gen
{
	Queue::Queue(u32 familyIndex, u32 queueIndex, vk::QueueFamilyProperties properties, vk::Bool32 canPresent)
	{
		m_handle = Device::self().getHandle().getQueue(familyIndex, queueIndex);
	}
} // namespace gen