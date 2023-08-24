// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

// TODO: Replace this with a proper implementation

#include "gen/application.hpp"
#include <numbers>

namespace gen
{

	Application::Application(const char * const appName, const u32 appVersion, mim::vec2i const & initialSize)
		: m_engine(std::make_unique<Engine>(appName, appVersion, initialSize))
	{
	}

	void Application::run()
	{
		while (!Window::getInstance().shouldClose())
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
	void Application::update(float dt)
	{
		Window::pollEvents();
	}

} // namespace gen
