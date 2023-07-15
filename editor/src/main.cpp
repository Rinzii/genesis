// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <log.hpp>

#include <iostream>
#include <stdexcept>

#include <logger/log.hpp>
#include <logger/instance.hpp>
#include <future>

int main()
{
	{
		using namespace std::chrono_literals;

		auto logger_instance = gen::refactor::logger::Instance{"test.log"};
		gen::refactor::logger::info("hello world");
		auto f = std::async(std::launch::async, [] { gen::refactor::logger::info("thread","42 from thread"); });
		std::this_thread::sleep_for(10ms);
		gen::refactor::logger::info("TestCategory", "hi there");
	}
    gen::Application app;

	gen::logger::log("Hello, world!");

	try {
		app.run();
	} catch (const std::exception& e) {
		//gen::logger::fatal(e.what());
        return 1;
	}

    return 0;
}

