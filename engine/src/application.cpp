// Copyright Ian Pike. All Rights Reserved.

// TODO: Replace this with a proper implementation

#include "application.hpp"
#include <numbers>

namespace gen
{

	void Application::run()
	{
		m_logger.debug("Testing logger: {:.2f} {}", std::numbers::pi_v<float>, "hello world");

		while (!m_window.shouldClose()) { gen::Window::pollEvents(); }
	}

} // namespace gen
