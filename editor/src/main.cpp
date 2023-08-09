// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <logger/instance.hpp>
#include <logger/log.hpp>
#include <inputs/inputs.hpp>

int main() {
	auto logger = gen::logger::Instance{};

	gen::Application app;
	gen::inputManager::init(app.getWindow());
	gen::logger::info("Hello, world!");

	try {
		app.run();
	} catch (std::exception const& e) {
		gen::logger::error(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
