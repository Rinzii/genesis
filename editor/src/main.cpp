// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <logger/instance.hpp>
#include <logger/log.hpp>

int main() {
	auto logger = gen::logger::Instance{};

	gen::Application app;

	try {
		app.run();
	} catch (std::exception const& e) {
		gen::logger::error(e.what());
        return 1;
	}

    return 0;
}
