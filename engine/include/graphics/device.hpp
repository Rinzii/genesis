// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "core.hpp"
#include "windowing/window.hpp"

// external
#include <vulkan/vulkan.hpp>

// std
#include <vector>

namespace gen
{
	struct Gpu
	{
		vk::PhysicalDevice physicalDevice{};
		vk::PhysicalDeviceProperties properties{};
		u32 queueFamily{};
	};

	class Device
	{
	public:
		Device(vk::Instance const & instance, vk::SurfaceKHR const & surface);
		~Device();

		Device(const Device &)			   = delete;
		Device(Device &&)				   = delete;
		Device & operator=(const Device &) = delete;
		Device & operator=(Device &&)	   = delete;

		/// Getters

		GEN_NODISCARD vk::Device getDevice() const { return m_device.get(); }
		GEN_NODISCARD Gpu getGpu() const { return m_gpu; }
		GEN_NODISCARD vk::Queue getGraphicsQueue() const { return m_graphicsQueue; }

	private:
		void selectPhysicalDevice(vk::Instance const & instance);
		void createLogicalDevice(vk::SurfaceKHR const & surface);
		void createCommandPoolAndBuffer();

		static u32 findQueueFamily(vk::PhysicalDevice const & physicalDevice, vk::SurfaceKHR const & surface);

		vk::UniqueDevice m_device{};
		Gpu m_gpu{};
		vk::Queue m_graphicsQueue{};
		vk::UniqueCommandPool m_commandPool{}; // TODO: Does a command buffer and a command pool belong to a device?
		vk::CommandBuffer m_commandBuffer{};

		Logger m_logger{"graphics"};
	};
} // namespace gen
