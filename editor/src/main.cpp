// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <log.hpp>
#include "inputs/controller.hpp"
#include <iostream>
#include <stdexcept>
#include "../../ext/src/glfw/include/GLFW/glfw3.h" //temp

int main()
{
    gen::Application app;
	gen::controllerManager::init();
	gen::logger::log("Hello, world!");

	try {
		while (!app.getWindow().shouldClose()) {
			gen::Window::pollEvents();
			gen::controllerManager::updateControllers();

			if (gen::controllerManager::getController(0) != nullptr)
				std::cout << glfwGetGamepadName(0) << std::endl;
			
		}
	} catch (const std::exception& e) {
		//gen::logger::fatal(e.what());
        return 1;
	}

    return 0;
}

