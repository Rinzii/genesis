// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <logger/instance.hpp>
#include <logger/log.hpp>

int main()
{
	try
	{
		auto logger = gen::logger::Instance{}; // Required to initialize the logger

		gen::Application app;
		app.run();
	}
	catch (std::exception const & e)
	{
		gen::logger::error(e.what());
		return 1;
	}

	return 0;
}
