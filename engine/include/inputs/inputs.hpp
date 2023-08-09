#pragma once
#include "controller.hpp"
#include "mouseAndKeyboard.hpp"
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
