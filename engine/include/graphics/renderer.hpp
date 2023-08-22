// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "core.hpp"
#include "graphics/device.hpp"
#include "graphics/swapchain.hpp"
#include "logger/log.hpp"
#include "windowing/window.hpp"

// external
#include <vulkan/vulkan.hpp>

namespace gen
{
	class Renderer
	{
	public:
		Renderer(const Window & window, std::string const & appName);
		~Renderer();

		Renderer(const Renderer &)			   = delete;
		Renderer(Renderer &&)				   = delete;
		Renderer & operator=(const Renderer &) = delete;
		Renderer & operator=(Renderer &&)	   = delete;

	private:
		void createInstance(const std::string & appName, const std::string & engineName, const gen::u32 & apiVersion);
		void createSurface(const Window & window);

		vk::UniqueInstance m_instance{};
		vk::UniqueSurfaceKHR m_surface{};
		Device m_device;
		Swapchain m_swapchain;

		Logger m_logger{"graphics"};
	};
} // namespace gen