// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "gen/core.hpp"
#include "gen/core/monoInstance.hpp"
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

	class Device : public MonoInstance<Device>
	{
	public:
		Device(const std::string & appName, u32 appVersion, const std::string & engineName, const u32 & apiVersion);
		~Device();

		Device(const Device &)			   = delete;
		Device(Device &&)				   = delete;
		Device & operator=(const Device &) = delete;
		Device & operator=(Device &&)	   = delete;

		/// Getters

		GEN_NODISCARD vk::Instance getInstance() const { return m_instance.get(); }
		GEN_NODISCARD vk::SurfaceKHR getSurface() const { return m_surface.get(); }
		GEN_NODISCARD vk::PhysicalDevice getPhysicalDevice() const { return m_gpu.physicalDevice; }
		GEN_NODISCARD vk::Device getDevice() const { return m_device.get(); }
		GEN_NODISCARD vk::Queue getQueue() const { return m_queue; }
		GEN_NODISCARD Gpu getGpu() const { return m_gpu; }
		GEN_NODISCARD vk::PhysicalDeviceProperties getProperties() const { return m_gpu.properties; }
		GEN_NODISCARD u32 getQueueFamily() const { return m_gpu.queueFamily; }

		/// Helpers

		GEN_NODISCARD bool waitForFence(vk::Fence fence, u64 timeout = std::numeric_limits<u64>::max()) const;
		bool submit(vk::SubmitInfo2 const & submitInfo, vk::Fence signal = {}) const;
		bool submitAndPresent(vk::SubmitInfo2 const & submitInfo, vk::Fence signal, vk::PresentInfoKHR const & presentInfo) const;

	private:
		void createInstance(const std::string & appName, u32 appVersion, const std::string & engineName, const u32 & apiVersion);
		void createSurface();
		void selectPhysicalDevice();
		void createLogicalDevice();

		/// Helpers

		static u32 findQueueFamily(vk::PhysicalDevice const & physicalDevice, vk::SurfaceKHR const & surface);

		vk::UniqueInstance m_instance{};
		vk::UniqueSurfaceKHR m_surface{};
		vk::UniqueDevice m_device{};
		vk::Queue m_queue{};

		Gpu m_gpu{};

		mutable std::mutex m_queueMutex{};
		Logger m_logger{"graphics"};
	};
} // namespace gen
