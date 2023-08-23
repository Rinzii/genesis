// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/graphics/renderer.hpp"
#include "mim/vec2.hpp"

namespace gen
{
	class Engine
	{
	public:
		Engine(const char * appName, const u32 appVersion, mim::vec2i const & initialSize);
		~Engine();

		Engine(const Engine &)			   = delete;
		Engine(Engine &&)				   = delete;
		Engine & operator=(const Engine &) = delete;
		Engine & operator=(Engine &&)	   = delete;

		/// Getters
		Window & window() { return m_window; }
		Renderer & renderer() { return m_renderer; }

	private:
		Window m_window;
		Renderer m_renderer;
	};
} // namespace gen