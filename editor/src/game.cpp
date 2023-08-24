// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "game.hpp"

namespace gen
{
	Game::Game(const char * appName, const u32 appVersion, const mim::vec2i & initialSize) : Application(appName, appVersion, initialSize)
	{
	}

	void Game::draw()
	{

		Application::draw(); // This is required for internal engine drawing
	}

	void Game::update(float dt)
	{

		Application::update(dt); // This is required for internal engine updating
	}

} // namespace gen
