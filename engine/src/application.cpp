// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

// TODO: Replace this with a proper implementation

#include "application.hpp"

namespace gen
{
	// possibly change this to instead use Update() and Draw() functions

	Application::Application(const std::string & appName)
		: m_window{ m_width, m_height, appName }, m_graphicsDevice{ appName }
	{
	}

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


} // namespace gen
