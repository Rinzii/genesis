// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>

#include <iostream>
#include <stdexcept>

#include <logger/log.hpp>
#include <logger/instance.hpp>
#include <future>

int main()
{
	auto logger = gen::logger::Instance{};

    gen::Application app;

	gen::logger::info("Hello, world!");

	try {
		app.run();
	} catch (const std::exception& e) {
		//gen::logger::fatal(e.what());
        return 1;
	}

    return 0;
}

