// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>

int main() {

	gen::Application app;
	
	try {
		app.run();
	}
	catch (std::exception const& e) {
		//add logger
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
