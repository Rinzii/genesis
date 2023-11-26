// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include "gen/rendering/vk/devices/physicalDevice.hpp"

#include "gen/rendering/vk/queue.hpp"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

#include <iosfwd>

namespace gen
{
	struct DeviceInfo
	{
		std::string name{};
		PhysicalDevice physicalDevice{};
		vk::DeviceSize totalLocalDevice{0};
		std::vector<vk::ExtensionProperties> extensions{};
	};

	class Device
	{
	public:
		Device(std::string name);
		~Device();

		Device(Device const &)			   = delete;
		Device(Device &&) noexcept		   = default;
		Device & operator=(Device const &) = delete;
		Device & operator=(Device &&)	   = delete;

		vk::Device getHandle() const { return m_handle.get(); }

	private:
		vk::UniqueDevice m_handle{nullptr};
		VmaAllocator m_allocator{nullptr};

		// TODO: Maybe handle multiple queues at a later date? Idk maybe not.

		vk::Queue m_graphicsQueue{nullptr};

		u32 m_graphicsFamilyIndex{0};
	};
} // namespace gen