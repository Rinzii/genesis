// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include "gen/util/version.hpp"

#include "gen/logger/log.hpp"
#include "gen/windowing/window.hpp"

#include "engine.hpp"
#include "time.hpp"

#include <memory>

namespace gen
{
	class Application
	{
	public:
		explicit Application(const char * appName, u32 appVersion, mim::vec2i const & initialSize);
		virtual ~Application() = default;

		virtual void run() final;

		virtual void draw();

		virtual void update(float dt);

		std::unique_ptr<Engine> m_engine;

		Logger m_logger{"application"};
	};

} // namespace gen
