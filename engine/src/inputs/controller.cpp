// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "inputs/controller.hpp"

namespace gen
{
	Controller::Controller(const int id)
		: m_id(id)
	{
		glfwGetJoystickAxes(m_id, &m_axesCount);
		glfwGetJoystickButtons(m_id, &m_buttonsCount);

		m_axes = std::vector<float>(m_axesCount, 0);
		m_buttons = std::vector<unsigned char>(m_buttonsCount, '0');
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

		for (int i = 0; i < m_axesCount; i++)
		{
			m_axes.push_back((axes[i] + 1) / 2);
		}
	}

	void Controller::storeValue(const unsigned char * buttons)
	{
		m_buttons.clear();

		for (int i = 0; i < m_buttonsCount; i++)
		{
			m_buttons.push_back(buttons[i]);
		}
	}
}
