// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/engine.hpp"

namespace gen
{
	Engine::Engine(const char * appName, const u32 appVersion, mim::vec2i const & initialSize)
		: m_window(std::make_unique<Window>(initialSize, appName)), m_renderer(std::make_unique<Renderer>(appName, appVersion))
	{
		m_logger.info("Engine created");
	}

} // namespace gen
