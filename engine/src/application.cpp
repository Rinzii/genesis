// Copyright Ian Pike. All Rights Reserved.

// TODO: Replace this with a proper implementation

#include "application.hpp"

namespace gen
{

	void Application::run()
	{
		while (!m_window.shouldClose()) { gen::Window::pollEvents(); }
	}

} // namespace gen
