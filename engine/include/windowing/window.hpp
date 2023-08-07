// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include <memory>
#include <string>

// TODO: Replace this with a proper implementation

// Start Forward declarations
struct GLFWwindow;

// End Forward declarations

namespace gen
{

	class Window
	{
	public:
		// Values mirrored from GLFW
		// https://www.glfw.org/docs/3.3/glfw3_8h.html#a2315b99a329ce53e6a13a9d46fd5ca88
		enum class CursorMode
		{
			Normal = 0x00034001,
			Hidden = 0x00034002,
			Disabled = 0x00034003
		};

		Window(int width, int height, std::string title);
		~Window();

		Window(const Window &)			   = delete;
		Window & operator=(const Window &) = delete;
		Window(Window &&)				   = default;
		Window & operator=(Window &&)	   = default;

		// Member functions

		GEN_NODISCARD bool shouldClose() const;
		static void pollEvents();

		// Getters

		GEN_NODISCARD int getWidth() const { return m_width; }
		GEN_NODISCARD int getHeight() const { return m_height; }
		GEN_NODISCARD const std::string & getTitle() const { return m_title; }
		GEN_NODISCARD CursorMode getCursorMode() const { return m_currentCursorMode; }
		GEN_NODISCARD GLFWwindow * getHandle() const { return m_window.get(); }

		// TODO: Currently these functions will break the window. Have not yet implemented window resizing.

		// Setters

		void setWidth(int width) { m_width = width; }
		void setHeight(int height) { m_height = height; }
		void setTitle(const std::string & title) { m_title = title; }
		void setCursorMode(CursorMode mode);

	private:

		// Callbacks
		static void callback_error(int error, const char * description);
		static void callback_cursor_position(GLFWwindow * window, double xPos, double yPos);

		int m_width;
		int m_height;
		std::string m_title;

		Window::CursorMode m_currentCursorMode { Window::CursorMode::Normal };


		struct Deleter
		{
			void operator()(GLFWwindow* ptr) const;
		};

		std::unique_ptr<GLFWwindow, Deleter> m_window;

	};

} // namespace gen
