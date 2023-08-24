// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/engine.hpp"

namespace gen
{
	Engine::Engine(const char * appName, const u32 appVersion, mim::vec2i const & initialSize)
		: m_window{initialSize, appName}, m_renderer{m_window, appName, appVersion}
	{
	}

} // namespace gen
