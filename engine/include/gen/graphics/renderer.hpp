// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "command.hpp"
#include "device.hpp"
#include "gen/core.hpp"
#include "gen/logger/log.hpp"
#include "gen/windowing/window.hpp"
#include "swapchain.hpp"

namespace gen
{
	class Renderer
	{
	public:
		Renderer(const Window & window, const char * const appName, const u32 appVersion);
		~Renderer() = default;

		Renderer(const Renderer &)			   = delete;
		Renderer(Renderer &&)				   = delete;
		Renderer & operator=(const Renderer &) = delete;
		Renderer & operator=(Renderer &&)	   = delete;

	private:
		Device m_device;
		Swapchain m_swapchain;
		CommandBufferPool m_commandBufferPool;

		Logger m_logger{"graphics"};
	};
} // namespace gen