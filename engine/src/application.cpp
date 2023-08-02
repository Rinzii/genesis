// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

// TODO: Replace this with a proper implementation

#include "application.hpp"

namespace gen {

	void Application::run() {
		initialize();
		gameLoop();
		shutdown();
    }
	void Application::initialize() {
		initializeGraphics();
	}
	void Application::gameLoop() {
		while (!m_window.shouldClose()) {
            Window::pollEvents();
        }

	}
	void Application::shutdown() {
	}

	void Application::initializeGraphics() {

	}

	} // namespace gen
