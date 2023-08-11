// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <logger/instance.hpp>
#include <logger/log.hpp>

int main()
{
	auto logger = gen::logger::Instance{};

	gen::Application app;

	gen::logger::general.info("Hello, world!");

	try
	{
		app.run();
	}
	catch (std::exception const & e)
	{
		gen::logger::general.error("{}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
