// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/core/monoInstance.hpp"

#include <vulkan/vulkan.hpp>

#include <iosfwd>


namespace gen
{
	class PhysicalDevice
	{
	public:
		PhysicalDevice();

		[[nodiscard]] vk::PhysicalDevice getHandle() const { return m_physicalDevice; }
		[[nodiscard]] vk::PhysicalDeviceProperties getProperty() const { return m_properties; }
		[[nodiscard]] vk::PhysicalDeviceFeatures getFeatures() const { return m_features; }
		[[nodiscard]] vk::PhysicalDeviceMemoryProperties getMemoryProperties() const { return m_memoryProperties; }


	private:

		std::string m_name;
		vk::PhysicalDevice m_physicalDevice;
		vk::PhysicalDeviceType m_type;
		vk::PhysicalDeviceProperties m_properties;
		vk::PhysicalDeviceFeatures m_features;
		vk::PhysicalDeviceMemoryProperties m_memoryProperties;
	};


} // namespace gen