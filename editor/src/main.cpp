// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <log.hpp>
#include "inputs/controller.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
    gen::Application app;
	gen::controllerManager::init();
	gen::logger::log("Hello, world!");

	try {
		while (!app.getWindow().shouldClose()) {
			gen::Window::pollEvents();
			gen::controllerManager::updateControllers();
			//gen::controllerManager::getController(0)->printKey();
		}
	} catch (const std::exception& e) {
		//gen::logger::fatal(e.what());
        return 1;
	}

    return 0;
}

