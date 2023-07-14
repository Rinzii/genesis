// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <log.hpp>

#include <iostream>
#include <stdexcept>

int main()
{
    gen::Application app;

	gen::logger::log("Hello, world!");

	try {
		app.run();
	} catch (const std::exception& e) {
		//gen::logger::fatal(e.what());
        return 1;
	}

    return 0;
}

