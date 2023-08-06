// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
#include <GLFW/glfw3.h>
#include <mim/vec2.hpp>
#include "base/config/compilerTraits.hpp"

namespace gen
{
	class Controller;

	namespace controllerManager
	{
		void init();

		void updateControllers();

		void gamePadCallback(int id, int event);

		Controller* getController(int i); //for testing purposes
	}

	class Controller
	{
	public:

		Controller(const int id);

		void update();

		constexpr void setActive(bool active) { m_active = active; }

		void setUserPointer(void* owner); // will be changed to a pointer to an entity later

		GEN_NODISCARD constexpr bool isActive() const { return m_active; }
		GEN_NODISCARD constexpr void* getUserPointer() const { return m_userPointer; }

		GEN_NODISCARD constexpr mim::vec2<float> getLeftJoystick() const { return { m_axes[GLFW_GAMEPAD_AXIS_LEFT_X], m_axes[GLFW_GAMEPAD_AXIS_LEFT_Y] }; }
		GEN_NODISCARD constexpr float getLeftJoystickX() const { return m_axes[GLFW_GAMEPAD_AXIS_LEFT_X]; }
		GEN_NODISCARD constexpr float getLeftJoystickY() const { return m_axes[GLFW_GAMEPAD_AXIS_LEFT_Y]; }
		GEN_NODISCARD constexpr bool getLeftJoystickButton() const { return m_buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB]; }

		GEN_NODISCARD constexpr mim::vec2<float> getRightJoystick() const { return { m_axes[GLFW_GAMEPAD_AXIS_RIGHT_X], m_axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] }; }
		GEN_NODISCARD constexpr float getRightJoystickX() const { return m_axes[GLFW_GAMEPAD_AXIS_RIGHT_X]; }
		GEN_NODISCARD constexpr float getRightJoystickY() const { return m_axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]; }
		GEN_NODISCARD constexpr bool getRightJoystickButton() const { return m_buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]; }

		GEN_NODISCARD constexpr float getLeftTrigger() const { return m_axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]; }
		GEN_NODISCARD constexpr float getRightTrigger() const { return m_axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]; }

		GEN_NODISCARD constexpr bool getLeftBumper() const { return m_buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]; }
		GEN_NODISCARD constexpr bool getRightBumper() const { return m_buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]; }

		GEN_NODISCARD constexpr bool getUp() const { return m_buttons[GLFW_GAMEPAD_BUTTON_Y]; }
		GEN_NODISCARD constexpr bool getDown() const { return m_buttons[GLFW_GAMEPAD_BUTTON_A]; }
		GEN_NODISCARD constexpr bool getLeft() const { return m_buttons[GLFW_GAMEPAD_BUTTON_X]; }
		GEN_NODISCARD constexpr bool getRight() const { return m_buttons[GLFW_GAMEPAD_BUTTON_B]; }

		GEN_NODISCARD constexpr bool getDPadUp() const { return m_buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]; }
		GEN_NODISCARD constexpr bool getDPadDown() const { return m_buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]; }
		GEN_NODISCARD constexpr bool getDPadLeft() const { return m_buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]; }
		GEN_NODISCARD constexpr bool getDPadRight() const { return m_buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]; }

		GEN_NODISCARD constexpr bool getGuide() const { return m_buttons[GLFW_GAMEPAD_BUTTON_GUIDE]; }
		GEN_NODISCARD constexpr bool getStart() const { return m_buttons[GLFW_GAMEPAD_BUTTON_START]; }

		GEN_NODISCARD constexpr int getID() const { return m_id; }

	private:

		int m_id;

		int m_axesCount { 0 };
		int m_buttonsCount { 0 };

		void* m_userPointer = nullptr;

		const float* m_axes = nullptr;
		const unsigned char* m_buttons = nullptr;

		bool m_active;
	};
}
