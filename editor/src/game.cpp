// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "game.hpp"

namespace gen
{
	Game::Game(const char * appName, const mim::vec2i & initialSize) : Application(appName, initialSize)
	{
	}

	void Game::run()
	{

		Application::run(); // This is required for internal engine running
	}

	void Game::draw()
	{

		Application::draw(); // This is required for internal engine drawing
	}

	void Game::update(double dt)
	{

		Application::update(dt); // This is required for internal engine updating
	}

} // namespace gen
