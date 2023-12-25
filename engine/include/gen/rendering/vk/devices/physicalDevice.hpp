// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/core/monoInstance.hpp"
#include "gen/rendering/vk/devices/instance.hpp"
#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>

#include <iosfwd>

namespace gen
{
	class PhysicalDevice
	{
	public:
		explicit PhysicalDevice(Instance & instance, vk::PhysicalDevice gpu);

		[[nodiscard]] vk::PhysicalDevice getHandle() const { return m_handle; }
		[[nodiscard]] vk::PhysicalDeviceProperties getProperties() const { return m_properties; }
		[[nodiscard]] vk::PhysicalDeviceFeatures getFeatures() const { return m_features; }
		[[nodiscard]] vk::PhysicalDeviceMemoryProperties getMemoryProperties() const { return m_memoryProperties; }
		[[nodiscard]] std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties() const { return m_queueFamilyProperties; }

		u32 getMemoryType(u32 bits, vk::MemoryPropertyFlags properties, vk::Bool32 * memoryTypeFound = nullptr) const;

	private:
		Instance & m_instance;

		std::string m_name;
		vk::PhysicalDevice m_handle{nullptr};
		vk::PhysicalDeviceProperties m_properties;
		vk::PhysicalDeviceType m_type;
		vk::PhysicalDeviceFeatures m_features;
		vk::PhysicalDeviceMemoryProperties m_memoryProperties;
		std::vector<vk::QueueFamilyProperties> m_queueFamilyProperties;
	};

} // namespace gen