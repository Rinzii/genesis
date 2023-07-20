// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "inputs/controller.hpp"

#include <vector>
#include <map>
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>

namespace gen
{
	namespace controllerManager
	{
		namespace
		{
			std::map<int, std::string> g_inputsToText = {
				{ GLFW_GAMEPAD_BUTTON_A, "A" },
				{ GLFW_GAMEPAD_BUTTON_B, "B" },
				{ GLFW_GAMEPAD_BUTTON_X, "X" },
				{ GLFW_GAMEPAD_BUTTON_Y, "Y" },
				{ GLFW_GAMEPAD_BUTTON_DPAD_UP,"DPadUp" },
				{ GLFW_GAMEPAD_BUTTON_DPAD_LEFT, "DPadLeft" },
				{ GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, "DPadRight" },
				{ GLFW_GAMEPAD_BUTTON_DPAD_DOWN, "DPadDown" },
				{ GLFW_GAMEPAD_BUTTON_LEFT_BUMPER, "LeftBumper" },
				{ GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER, "RightBumper" },
				{ GLFW_GAMEPAD_BUTTON_LEFT_THUMB, "LeftJoystickPress" },
				//{ GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,"LeftTrigger" },
				//{ GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,"RightTrigger" },
				{ GLFW_GAMEPAD_BUTTON_START,"Start" },
				{ GLFW_GAMEPAD_BUTTON_GUIDE,"Guide" },
			};

			std::vector<Controller> g_connectedControllers; //Max 16 (because of GLFW)
		}

		void init()
		{
			glfwSetJoystickCallback(gamePadCallback);

			int alreadyConnectedGamePadCount = 0;

			while (glfwJoystickPresent(alreadyConnectedGamePadCount) != 0)
			{
				if (glfwJoystickIsGamepad(alreadyConnectedGamePadCount))
					g_connectedControllers.emplace_back(alreadyConnectedGamePadCount);
				alreadyConnectedGamePadCount++;

				if (alreadyConnectedGamePadCount == GLFW_JOYSTICK_LAST)
					break;
			}
		}

		void updateControllers()
		{
			for (auto & connectedController : g_connectedControllers)
				connectedController.update();
		}

		void gamePadCallback(int id, int event)
		{
			switch (event)
			{
			case GLFW_CONNECTED:
				if (glfwJoystickIsGamepad(id))
					g_connectedControllers.emplace_back(id);
				break;

			case GLFW_DISCONNECTED:
				for (int i = 0; i < g_connectedControllers.size(); i++)
					if (g_connectedControllers[i].getID() == id)
						g_connectedControllers.erase(g_connectedControllers.begin() + i);
				break;
			}
		}

		Controller * getController(int i)
		{
			if (i < g_connectedControllers.size())
				return &g_connectedControllers[i];

			return nullptr;
		}
	}

	Controller::Controller(const int id)
		: m_id(id)
	{}

	void Controller::update()
	{
		m_axes = glfwGetJoystickAxes(m_id, &m_axesCount);
		m_buttons = glfwGetJoystickButtons(m_id, &m_buttonsCount);
	}

	void Controller::setUserPointer(void * owner) const
	{
		glfwSetJoystickUserPointer(m_id, owner);
	}

	void Controller::printKey() const
	{
		for (int i = 0; i < m_buttonsCount; i++)
			if (m_buttons[i] == GLFW_PRESS)
				std::cout << controllerManager::g_inputsToText[i] << std::endl;

		//for (int i = 0; i < m_axesCount; i++)
		//	if (m_axes[i] == GLFW_PRESS)
		//		std::cout << controllerManager::g_inputsToText[i] << std::endl;
	}

	mim::vec2<float> Controller::getLeftJoystick() const
	{
		return { m_axes[GLFW_GAMEPAD_AXIS_LEFT_X], m_axes[GLFW_GAMEPAD_AXIS_LEFT_Y] };
	}

	float Controller::getLeftJoystickX() const
	{
		return m_axes[GLFW_GAMEPAD_AXIS_LEFT_X];
	}

	float Controller::getLeftJoystickY() const
	{
		return m_axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
	}

	bool Controller::getLeftJoystickButton() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB];
	}

	mim::vec2<float> Controller::getRightJoystick() const
	{
		return { m_axes[GLFW_GAMEPAD_AXIS_RIGHT_X], m_axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] };
	}

	float Controller::getRightJoystickX() const
	{
		return m_axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
	}

	float Controller::getRightJoystickY() const
	{
		return m_axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
	}

	bool Controller::getRightJoystickButton() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB];
	}

	float Controller::getLeftTrigger() const
	{
		return m_axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
	}

	float Controller::getRightTrigger() const
	{
		return m_axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
	}

	bool Controller::getLeftBumper() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER];
	}

	bool Controller::getRightBumper() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER];
	}

	bool Controller::getUp() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_Y];
	}

	bool Controller::getDown() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_A];
	}

	bool Controller::getLeft() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_X];
	}

	bool Controller::getRight() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_B];
	}

	bool Controller::getDPadUp() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT];
	}

	bool Controller::getDPadDown() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT];
	}

	bool Controller::getDPadLeft() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
	}

	bool Controller::getDPadRight() const
	{
		return m_buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP];
	}

	int Controller::getID() const
	{
		return m_id;
	}
}
