// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include "util/version.hpp"

#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"
#include "windowing/window.hpp"

namespace gen
{

	class Application
	{
	public:
		explicit Application(const char* appName, const int initailWidth, const int initialHeight);

		void run();

	private:
		void gameLoop();
		void shutdown();

		Window m_window;
		GraphicsDevice m_graphicsDevice;
	};

} // namespace gen
