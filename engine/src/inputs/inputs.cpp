// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "inputs/inputs.hpp"
#include "application.hpp"

#include <GLFW/glfw3.h>

namespace gen
{
	namespace inputManager
	{
		namespace
		{
			//variables
			std::vector<Controller> g_connectedControllers; //Max 16 (because of GLFW)
			PCPeripherals g_peripherals;

			//"private" functions
			void addPreviouslyConnectedControllers()
			{
				int alreadyConnectedGamePadCount = 0;

				while (glfwJoystickPresent(alreadyConnectedGamePadCount) != 0)
				{
					if (glfwJoystickIsGamepad(alreadyConnectedGamePadCount))
					{
						g_connectedControllers.emplace_back(alreadyConnectedGamePadCount);
					}

					alreadyConnectedGamePadCount++;

					if (alreadyConnectedGamePadCount == GLFW_JOYSTICK_LAST)
					{
						break;
					}
				}
			}

			void gamePadCallback(int id, int event)
			{
				switch (event)
				{
				case GLFW_CONNECTED:
					if (glfwJoystickIsGamepad(id))
					{
						for (auto& connectedController : g_connectedControllers)
						{
							if (connectedController.getID() == id)
							{
								connectedController.setActive(true);
								connectedController.setUserPointer(connectedController.getUserPointer());
							}
						}

						g_connectedControllers.emplace_back(id);
					}
					break;

				case GLFW_DISCONNECTED:
					for (auto& connectedController : g_connectedControllers)
					{
						if (connectedController.getID() == id)
						{
							connectedController.setActive(false);
						}
					}
					break;

				default:
					return;
				}
			}
		}

		void init(Window& window)
		{
			glfwSetJoystickCallback(gamePadCallback);
			glfwSetKeyCallback(&window.GetGLFWWindow(), Keyboard::keyCallback);

			addPreviouslyConnectedControllers();
		}

		void updateControllers()
		{
			for (auto& connectedController : g_connectedControllers)
			{
				if (connectedController.isActive())
				{
					connectedController.update();
				}
			}
		}

		Controller* getController(int i)
		{
			if (i < g_connectedControllers.size() && g_connectedControllers[i].isActive())
			{
				return &g_connectedControllers[i];
			}

			return nullptr;
		}
	}
}
