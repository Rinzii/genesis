// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <logger/instance.hpp>
#include <logger/log.hpp>

static constexpr int width { 800 };
static constexpr int height { 600 };
static constexpr const char* appName { "Genesis Game" };

int main()
{
	try
	{
		auto logger = gen::logger::Instance{}; // Required to initialize the logger

		gen::Application app{ appName, width, height };
		app.run();
	}
	catch (std::exception const & e)
	{
		gen::logger::error(e.what());
		return 1;
	}

	return 0;
}
