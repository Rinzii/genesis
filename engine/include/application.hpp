// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"
#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"
#include "windowing/window.hpp"

namespace gen
{

	class Application
	{
	public:
		void run();

	private:
		void gameLoop();
		void shutdown();

	public:
		static constexpr u32 m_width  = 800;
		static constexpr u32 m_height = 600;

	private:
		Window m_window{ m_width, m_height, "Genesis Engine" };
		GraphicsDevice m_graphicsDevice{ m_window };
	};

} // namespace gen
