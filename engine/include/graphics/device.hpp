// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"
#include "windowing/window.hpp"

#include <vector>

#include <vulkan/vulkan.hpp>

namespace gen
{

	class GraphicsDevice
	{
	public:
		GraphicsDevice(const GraphicsDevice &)			   = delete;
		GraphicsDevice & operator=(const GraphicsDevice &) = delete;
		GraphicsDevice(GraphicsDevice &&)				   = delete;
		GraphicsDevice & operator=(GraphicsDevice &&)	   = delete;


		explicit GraphicsDevice(const Window & window, std::string const & appName );
		~GraphicsDevice() = default;

	private:
		void createInstance(const std::string & appName, const std::string & engineName, const gen::u32 & apiVersion);
		void createSurface(const Window & window);
		void createDevice();


		vk::UniqueInstance m_instance;
		vk::UniqueSurfaceKHR m_surface;
		vk::PhysicalDevice m_physicalDevice;
		vk::UniqueDevice m_device;

	};
} // namespace gen
