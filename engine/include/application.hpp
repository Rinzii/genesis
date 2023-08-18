// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include "util/version.hpp"

#include <logger/log.hpp>
#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"
#include "windowing/window.hpp"

namespace gen
{
	class Application
	{
	public:
		explicit Application(const char * appName, mim::vec2i const & initialSize)
			: m_window{initialSize, appName},
			  m_graphicsDevice{m_window, appName},

		{}


		void run();

	private:
		void gameLoop();
		void shutdown();

		Window m_window;
		GraphicsDevice m_graphicsDevice;
		GraphicsPipeline m_graphicsPipeline;


		Logger m_logger{"application"};
	};

} // namespace gen
