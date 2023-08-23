// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include "gen/util/version.hpp"

#include "gen/logger/log.hpp"
#include "gen/windowing/window.hpp"

#include "engine.hpp"
#include "time.hpp"

namespace gen
{
	class Application
	{
	public:
		explicit Application(const char * appName, mim::vec2i const & initialSize);
		virtual ~Application();

		virtual void run() final;

		virtual void draw();

		virtual void update(double dt);

		Engine m_engine;

		Logger m_logger{"application"};
	};

} // namespace gen