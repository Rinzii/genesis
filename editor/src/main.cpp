// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <logger/instance.hpp>
#include <logger/log.hpp>

int main() {
	// We don't want to initialize the logger in release mode.
#ifndef GEN_NDEBUG
	auto logger = gen::logger::Instance{}; // Required to initialize the logger
#endif

	gen::Application app;

	try {
		app.run();
	} catch (std::exception const& e) {
		gen::logger::error(e.what());
        return 1;
	}

    return 0;
}
