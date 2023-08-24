// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "gen/core.hpp"
#include "gen/windowing/window.hpp"

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
		Device(const std::string & appName, const u32 appVersion, const std::string & engineName, const u32 & apiVersion, const Window & window);
		~Device();

		Device(const Device &)			   = delete;
		Device(Device &&)				   = delete;
		Device & operator=(const Device &) = delete;
		Device & operator=(Device &&)	   = delete;

		/// Getters

		GEN_NODISCARD vk::Instance getInstance() const { return m_instance.get(); }
		GEN_NODISCARD vk::SurfaceKHR getSurface() const { return m_surface.get(); }
		GEN_NODISCARD vk::Device getDevice() const { return m_device.get(); }
		GEN_NODISCARD Gpu getGpu() const { return m_gpu; }
		GEN_NODISCARD vk::Queue getGraphicsQueue() const { return m_graphicsQueue; }

	private:
		void createInstance(const std::string & appName, const u32 appVersion, const std::string & engineName, const u32 & apiVersion);
		void createSurface(const Window & window);
		void selectPhysicalDevice();
		void createLogicalDevice();

		/// Helpers

		static u32 findQueueFamily(vk::PhysicalDevice const & physicalDevice, vk::SurfaceKHR const & surface);

		vk::UniqueInstance m_instance{};
		vk::UniqueSurfaceKHR m_surface{};
		vk::UniqueDevice m_device{};
		Gpu m_gpu{};
		vk::Queue m_graphicsQueue{};

		Logger m_logger{"graphics"};
	};
} // namespace gen
