// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <game/game.hpp>
#include <gen/logger/instance.hpp>
#include <gen/logger/log.hpp>
#include <gen/util/version.hpp>

// TODO: Replace this with a config file. At least for the startup window size.
static constexpr const char * appName{"Genesis Game - Editor"};
static constexpr gen::Version appVersion{0, 0, 1};
static constexpr mim::vec2i startingWindowSize{800, 600};
static constexpr const char * logFile{"genesis.log"};

int main()
{
	// TODO: Make this be set by a config file.
	auto config = gen::logger::Config{};

	// Required to initialize the logger for the application. This must also stay outside the try/catch block.
	auto logger = gen::logger::Instance{logFile, config};

	try
	{
		gen::Game app{appName, appVersion.getVersion(), startingWindowSize};
		app.run();
	}
	catch (std::exception const & e)
	{
		gen::logger::general.error("{}", e.what());
		return 1;
	}

	return 0;
}
