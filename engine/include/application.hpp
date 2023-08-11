// Copyright Ian Pike. All Rights Reserved.

#pragma once

#include <logger/log.hpp>
#include "windowing/window.hpp"

namespace gen
{

	class Application
	{
	public:
		void run();

		// This is all kinda bad but it is gonna be replaced later
		// Just using it for testing the rendering pipeline
		static constexpr u32 m_width  = 800;
		static constexpr u32 m_height = 600;

	private:
		Window m_window{800, 600, "Genesis Engine"};

		Logger m_logger{"Application"};
	};

} // namespace gen
