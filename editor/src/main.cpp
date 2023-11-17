// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <game/game.hpp>
#include <gen/logger/instance.hpp>
#include <gen/logger/log.hpp>
#include <gen/util/version.hpp>

#include <iostream>

// temp
#include "gen/io/fileUtils.hpp"

// TODO: Replace this with a config file. At least for the startup window size.
static constexpr const char * appName{"Genesis Game - Editor"};
static constexpr gen::Version appVersion{0, 0, 1};
static constexpr mim::vec2i startingWindowSize{800, 600};

int main()
{
	try
	{
		auto logger = gen::logger::Instance{}; // Required to initialize the logger

		gen::logger::general.log("Current executable path: {}", gen::io::getExecutablePath().string());

		gen::Game app{appName, appVersion.getVersion(), startingWindowSize};
		app.run();
	}
	catch (std::exception const & e)
	{
		// TODO: Logger does not work inside of a catch block. Need to fix this.
		std::cout << "Exception: " << e.what() << '\n';
		return 1;
	}

	return 0;
}
