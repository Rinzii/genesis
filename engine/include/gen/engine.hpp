// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/graphics/renderer.hpp"
#include "mim/vec2.hpp"

#include "gen/core/monoInstance.hpp"

#include <memory>

namespace gen
{
	class Engine : public MonoInstance<Engine>
	{
	public:
		struct Settings
		{

			bool vsync					  = false;
			Window::ScreenMode screenMode = Window::ScreenMode::eWindowed;
		};

		Engine(const char * appName, u32 appVersion, mim::vec2i const & initialSize);
		~Engine() = default;

		Engine(const Engine &)			   = delete;
		Engine(Engine &&)				   = delete;
		Engine & operator=(const Engine &) = delete;
		Engine & operator=(Engine &&)	   = delete;

	private:
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;

		Logger m_logger{"engine"};
	};
} // namespace gen