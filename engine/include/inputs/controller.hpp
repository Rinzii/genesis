// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
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

		void setUserPointer(void* owner) const; // will be changed to a pointer to an entity later

		void printKey() const;

		GEN_NODISCARD mim::vec2<float> getLeftJoystick() const;
		GEN_NODISCARD float getLeftJoystickX() const;
		GEN_NODISCARD float getLeftJoystickY() const;

		GEN_NODISCARD mim::vec2<float> getRightJoystick() const;
		GEN_NODISCARD float getRightJoystickX() const;
		GEN_NODISCARD float getRightJoystickY() const;

		GEN_NODISCARD bool getUp() const;
		GEN_NODISCARD bool getDown() const;
		GEN_NODISCARD bool getLeft() const;
		GEN_NODISCARD bool getRight() const;

		GEN_NODISCARD bool getDPadUp() const;
		GEN_NODISCARD bool getDPadDown() const;
		GEN_NODISCARD bool getDPadLeft() const;
		GEN_NODISCARD bool getDPadRight() const;

		GEN_NODISCARD int getID() const;

	//private:

		int m_id;

		int m_axesCount = 0;
		int m_buttonsCount = 0;

		const float* m_axes = nullptr;
		const unsigned char* m_buttons = nullptr;
	};
}
