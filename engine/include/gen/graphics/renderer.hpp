// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "command.hpp"
#include "device.hpp"
#include "gen/core.hpp"
#include "gen/core/monoInstance.hpp"
#include "gen/logger/log.hpp"
#include "gen/windowing/window.hpp"
#include "swapchain.hpp"

#include <memory>

namespace gen
{
	class Renderer : public MonoInstance<Renderer>
	{
	public:
		Renderer(const char * appName, u32 appVersion);
		~Renderer() = default;

		Renderer(const Renderer &)			   = delete;
		Renderer(Renderer &&)				   = delete;
		Renderer & operator=(const Renderer &) = delete;
		Renderer & operator=(Renderer &&)	   = delete;

	private:
		std::unique_ptr<Device> m_device;
		std::unique_ptr<Swapchain> m_swapchain;
		std::unique_ptr<CommandBufferPool> m_commandBufferPool;

		Logger m_logger{"graphics"};
	};
} // namespace gen