// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

// TODO: Replace this with a proper implementation

#include "gen/application.hpp"

#include <GLFW/glfw3.h>

namespace gen
{

	Application::Application(const char * const appName, const u32 appVersion, mim::vec2i const & initialSize)
	{
		m_engine = std::make_unique<Engine>(appName, appVersion, initialSize);
		m_window = std::make_unique<RenderWindow>(initialSize.x, initialSize.y, appName);

		glfwSetWindowUserPointer(m_window->getHandle(), this);
	}

	void Application::run()
	{
		while (!m_window->shouldClose())
		{
			update(Time::GetDeltaTime());
			draw();
		}
	}

	// Internal draw function that should be overridden by the user for game specific drawing
	void Application::draw()
	{
		m_engine->render();
	}

	// Internal update function that should be overridden by the user for game specific updating
	void Application::update(float dt)
	{
		Window::pollEvents();
	}
	RenderWindow & Application::getWindow() const
	{
		assert(m_window && "Window is null");
		return *m_window;
	}

} // namespace gen
