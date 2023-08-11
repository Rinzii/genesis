// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

// TODO: Replace this with a proper implementation

#include "application.hpp"
#include <numbers>

namespace gen
{
	// possibly change this to instead use Update() and Draw() functions

	void Application::run()
	{
		gameLoop();
		shutdown();
	}

	void Application::gameLoop()
	{
		while (!m_window.shouldClose()) { Window::pollEvents(); }
	}

	void Application::shutdown()
	{
	}

	Application::Application(const char * appName, mim::vec2i const& initialSize)
	    : m_window{ initialSize, appName }, m_graphicsDevice{ m_window, appName }
	{
	}


} // namespace gen
