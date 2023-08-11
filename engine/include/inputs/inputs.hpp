// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
#include "controller.hpp"
#include "pcPeripherals.hpp"
#include "windowing/window.hpp"

namespace gen
{
	namespace inputManager
	{
		void init(Window& window);

		void updateControllers();

		gen::Controller* getController(int i); //for testing purposes
	}
}
