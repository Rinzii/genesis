// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "core.hpp"
#include "windowing/window.hpp"

// external
#include <vulkan/vulkan.hpp>

// std
#include <vector>
#include <optional>

namespace gen
{
	struct Gpu
	{
		vk::PhysicalDevice physicalDevice{};
		vk::PhysicalDeviceProperties properties{};
		u32 queueFamily{};
	};


	struct QueueFamilyIndices
	{
		std::optional<u32> graphicsFamily;
		std::optional<u32> transferFamily;
		std::optional<u32> presentFamily;

		GEN_NODISCARD bool isComplete() const
		{
			return graphicsFamily.has_value() && transferFamily.has_value() && presentFamily.has_value();
		}
	};

	class GraphicsDevice
	{
	public:
		GraphicsDevice(const GraphicsDevice &)			   = delete;
		GraphicsDevice & operator=(const GraphicsDevice &) = delete;
		GraphicsDevice(GraphicsDevice &&)				   = delete;
		GraphicsDevice & operator=(GraphicsDevice &&)	   = delete;


		explicit GraphicsDevice(const Window & window, std::string const & appName );
		~GraphicsDevice();

		/// Getters

		GEN_NODISCARD vk::Instance const & getInstance() const { return m_instance.get(); }
		GEN_NODISCARD vk::SurfaceKHR const & getSurface() const { return m_surface.get(); }
		GEN_NODISCARD vk::PhysicalDevice const & getPhysicalDevice() const { return m_gpu.physicalDevice; }
		GEN_NODISCARD vk::Device const & getDevice() const { return m_device.get(); }

		/// Setters



	private:
		void createInstance(const std::string & appName, const std::string & engineName, const gen::u32 & apiVersion);
		void createSurface(const Window & window);
		void pickPhysicalDevice();
		void createLogicalDevice();

		// helpers

		static u32 findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface);


		vk::UniqueInstance m_instance;
		vk::UniqueSurfaceKHR m_surface;
		Gpu m_gpu;
		vk::UniqueDevice m_device;
		vk::Queue m_graphicsQueue;




	};
} // namespace gen
