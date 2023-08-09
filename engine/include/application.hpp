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
		explicit Application(std::string const & appName);

		void run();

	private:
		void gameLoop();
		void shutdown();


	public:
		static constexpr u32 m_width  = 800;
		static constexpr u32 m_height = 600;


	private:
		Window m_window;
		GraphicsDevice m_graphicsDevice;
	};

} // namespace gen
