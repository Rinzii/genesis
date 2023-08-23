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
		Device m_device;
		Swapchain m_swapchain;

		Logger m_logger{"graphics"};
	};
} // namespace gen