// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/vk/devices/physicalDevice.hpp"

namespace gen
{
	PhysicalDevice::PhysicalDevice(vk::PhysicalDevice gpu)
		: m_physicalDevice{gpu}
	{
	}
} // namespace gen