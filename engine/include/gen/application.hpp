// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include "gen/util/version.hpp"

#include "gen/logger/log.hpp"
#include "gen/rendering/renderWindow.hpp"

#include "gen/engine.hpp"
#include "gen/time.hpp"

#include <memory>

namespace gen
{
	class Application : public MonoInstance<Application>
	{
	public:
		explicit Application(const char * appName, u32 appVersion, mim::vec2i const & initialSize);
		virtual ~Application() = default;

		Application(const Application &)			 = delete;
		Application(Application &&)					 = delete;
		Application & operator=(const Application &) = delete;
		Application & operator=(Application &&)		 = delete;

		virtual void run() final;

		virtual void draw();

		virtual void update(float dt);

		[[nodiscard]] RenderWindow & getWindow() const;

	private:
		std::unique_ptr<RenderWindow> m_window;
		std::unique_ptr<Engine> m_engine;

		Logger m_logger{"application"};
	};

} // namespace gen
