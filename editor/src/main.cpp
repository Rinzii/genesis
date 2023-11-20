// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <logger/instance.hpp>
#include <logger/log.hpp>

// TODO: Replace this with a config file. At least for the startup window size.
static constexpr const char * appName{"Genesis Game"};
static constexpr mim::vec2i startingWindowSize{800, 600};

int main()
{
	// TODO: Make this be set by a config file.
	auto config = gen::logger::Config{};
#ifdef GEN_DEBUG
	config.verbose = true;
#endif

	// Required to initialize the logger for the application. This must also stay outside the try/catch block.
	auto logger = gen::logger::Instance{"genesis.log", config};

	try
	{

		gen::Application app{appName, startingWindowSize};
		app.run();
	}
	catch (std::exception const & e)
	{
		gen::logger::general.error("{}", e.what());
		return 1;
	}

	return 0;
}
