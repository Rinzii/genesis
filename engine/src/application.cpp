// Copyright Ian Pike. All Rights Reserved.

// TODO: Replace this with a proper implementation

#include "application.hpp"

namespace gen {

	void Application::run() {
        init();
		gameLoop();
		shutdown();
    }
	void Application::init() {
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
