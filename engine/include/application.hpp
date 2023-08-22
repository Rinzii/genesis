// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include "util/version.hpp"

#include <logger/log.hpp>
#include "graphics/device.hpp"
#include "windowing/window.hpp"

namespace gen
{
	class Application
	{
	public:
		explicit Application(const char * appName, mim::vec2i const & initialSize);

		void run();

	private:
		void gameLoop();
		void shutdown();

		void poll();
		void render();

		Window m_window;
		GraphicsDevice m_graphicsDevice;

		vk::UniqueCommandPool m_commandPool{};

		// TODO: double buffering
		struct
		{
			vk::CommandBuffer cmd{};
			vk::UniqueSemaphore acquire{};
			vk::UniqueSemaphore present{};
			vk::UniqueFence ready{};
		} m_sync{};

		Logger m_logger{"application"};
	};

} // namespace gen
