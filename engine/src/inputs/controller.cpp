// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/inputs/controller.hpp"

namespace gen
{
	namespace controllerManager
	{
		namespace
		{
			std::vector<Controller> g_connectedControllers; // Max 16 (because of GLFW)
		}

		void init()
		{
			glfwSetJoystickCallback(gamePadCallback);

			int alreadyConnectedGamePadCount = 0;

			while (glfwJoystickPresent(alreadyConnectedGamePadCount) != 0)
			{
				// NOLINTNEXTLINE(readability-implicit-bool-conversion)
				if (glfwJoystickIsGamepad(alreadyConnectedGamePadCount)) { g_connectedControllers.emplace_back(alreadyConnectedGamePadCount); }

				alreadyConnectedGamePadCount++;

				if (alreadyConnectedGamePadCount == GLFW_JOYSTICK_LAST) { break; }
			}
		}

		void updateControllers()
		{
			for (auto & connectedController : g_connectedControllers)
			{
				if (connectedController.isActive()) { connectedController.update(); }
			}
		}

		void gamePadCallback(int id, int event)
		{
			switch (event)
			{
			case GLFW_CONNECTED:
				if (glfwJoystickIsGamepad(id)) // NOLINT(readability-implicit-bool-conversion)
				{
					for (auto & connectedController : g_connectedControllers)
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
				for (auto & connectedController : g_connectedControllers)
				{
					if (connectedController.getID() == id) { connectedController.setActive(false); }
				}
				break;

			default: return;
			}
		}

		Controller * getController(int i)
		{
			const auto index = static_cast<const size_t>(i);
			if (index < g_connectedControllers.size() && g_connectedControllers[index].isActive()) { return &g_connectedControllers[index]; }

			return nullptr;
		}
	} // namespace controllerManager

	Controller::Controller(const int id) : m_id(id)
	{
		glfwGetJoystickAxes(m_id, &m_axesCount);
		glfwGetJoystickButtons(m_id, &m_buttonsCount);

		m_axes	  = std::vector<float>(static_cast<unsigned long>(m_axesCount), 0);
		m_buttons = std::vector<unsigned char>(static_cast<unsigned long>(m_buttonsCount), '0');
	}

	void Controller::update()
	{
		storeValue(glfwGetJoystickAxes(m_id, &m_axesCount));
		storeValue(glfwGetJoystickButtons(m_id, &m_buttonsCount));
	}

	void Controller::setUserPointer(void * owner)
	{
		glfwSetJoystickUserPointer(m_id, owner);
		m_userPointer = owner;
	}

	void Controller::storeValue(const float * axes)
	{
		m_axes.clear();

		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		for (int i = 0; i < m_axesCount; i++) { m_axes.push_back((axes[i] + 1) / 2); }
	}

	void Controller::storeValue(const unsigned char * buttons)
	{
		m_buttons.clear();

		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		for (int i = 0; i < m_buttonsCount; i++) { m_buttons.push_back(buttons[i]); }
	}
} // namespace gen
