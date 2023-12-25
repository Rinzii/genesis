// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/vk/devices/physicalDevice.hpp"

#include <limits>

namespace gen
{
	PhysicalDevice::PhysicalDevice(Instance & instance, vk::PhysicalDevice gpu)
	{
	}

	PhysicalDevice::PhysicalDevice(vk::PhysicalDevice gpu) : m_handle{gpu}
	{
		m_properties			= m_handle.getProperties();
		m_type					= m_handle.getProperties().deviceType;
		m_features				= m_handle.getFeatures();
		m_memoryProperties		= m_handle.getMemoryProperties();
		m_queueFamilyProperties = m_handle.getQueueFamilyProperties();

		m_name = static_cast<std::string>(m_properties.deviceName);
	}

	u32 PhysicalDevice::getMemoryType(u32 bits, vk::MemoryPropertyFlags properties, vk::Bool32 * memoryTypeFound) const
	{
		// Iterate through the available memory types
		for (u32 i = 0; i < m_memoryProperties.memoryTypeCount; ++i)
		{
			// Check if the current memory type is supported (bit is set in 'bits' parameter)
			if ((bits & 1) == 1)
			{
				// Check if the memory type satisfies the required properties
				if ((m_memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					// If a matching memory type is found and memoryTypeFound is provided, set its value to True
					if (memoryTypeFound != nullptr) { *memoryTypeFound = vk::True; }
					// Return the index of the found memory type
					return i;
				}
			}
			// Move to the next bit in 'bits'
			bits >>= 1;
		}

		// If no matching memory type is found and memoryTypeFound is provided, set its value to False
		if (memoryTypeFound != nullptr)
		{
			*memoryTypeFound = vk::False;
			// Return an invalid index to indicate failure
			return std::numeric_limits<u32>::max();
		}

		// If no matching memory type is found and memoryTypeFound is not provided, throw a runtime error
		throw std::runtime_error("Could not find a matching memory type");
	}

} // namespace gen