// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

// TODO: Replace this with a proper implementation

#include "gen/application.hpp"
#include <numbers>

namespace gen
{

	Application::Application(const char * appName, mim::vec2i const & initialSize) : m_engine{appName, initialSize}
	{
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (!m_engine.window().shouldClose())
		{
			update(Time::GetDeltaTime());
			draw();
		}
	}

	// Internal draw function that should be overridden by the user for game specific drawing
	void Application::draw()
	{
	}

	// Internal update function that should be overridden by the user for game specific updating
	void Application::update(double dt) // NOLINT
	{
		Window::pollEvents();
	}

} // namespace gen
