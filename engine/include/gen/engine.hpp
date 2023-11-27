// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/rendering/renderer.hpp"
#include "gen/system/types.hpp"
#include "mim/vec2.hpp"

#include "gen/windowing/window.hpp"

#include "gen/core/monoInstance.hpp"

#include <memory>
#include <string>

namespace gen
{
	class Engine : public MonoInstance<Engine>
	{
		static constexpr std::string_view s_engineName { "Genesis Engine" };

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

		void render();


		// Getters
		[[nodiscard]] static std::string_view getEngineName() { return s_engineName; }

	private:
		std::unique_ptr<Renderer> m_renderer;

		Logger m_logger{"engine"};
	};
} // namespace gen