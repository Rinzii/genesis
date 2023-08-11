// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

// internal
#include "windowing/window.hpp"

// external
#include <GLFW/glfw3.h>

// std
#include <cassert>
#include <format>

namespace gen
{

	Window::Window(const mim::vec2i extent, const char * title) // NOLINT(performance-unnecessary-value-param)
		: m_extent{extent}										// NOLINT(cppcoreguidelines-pro-type-member-init)
	{

		if (!glfwInit()) // NOLINT(readability-implicit-bool-conversion)
		{
			m_logger.error("Failed to initialize GLFW");
			throw std::runtime_error("Failed to initialize GLFW");
		}

		// Set window hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // TODO: Later add support for resizing of windows.

		// Create window
		GLFWwindow * window_ = glfwCreateWindow(static_cast<int>(m_extent.x), static_cast<int>(m_extent.y), title, nullptr, nullptr);

		m_window = std::unique_ptr<GLFWwindow, Deleter>(window_);

		// Check if window was created
		if (!m_window)
		{
			m_logger.error("Failed to create GLFW window");
			throw std::runtime_error("Failed to create GLFW window");
		}

		// Set modes
		glfwSetInputMode(m_window.get(), GLFW_CURSOR, static_cast<int>(m_currentCursorMode));

		// Set user pointer
		glfwSetWindowUserPointer(m_window.get(), this);

		// Set callbacks
		glfwSetErrorCallback(callback_error);
		glfwSetWindowSizeCallback(m_window.get(), callback_window_size);

		// Report successful window creation
		m_logger.info("Window constructed");
	}

	Window::~Window()
	{
		m_logger.info("Window destructed");
	}

	bool Window::shouldClose() const
	{
		return glfwWindowShouldClose(m_window.get()) != 0;
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}

	/// Getters

	mim::vec2i Window::getExtent()
	{
		// Query glfw for the window size just in case it has changed
		return m_extent;
	}

	int Window::getWidth() const
	{
		// Query glfw for the window size just in case it has changed
		return m_extent.x;
	}

	int Window::getHeight() const
	{
		// Query glfw for the window size just in case it has changed
		return m_extent.y;
	}

	Window::CursorMode Window::getCursorMode() const
	{
		return m_currentCursorMode;
	}

	GLFWwindow * Window::getHandle() const
	{
		return m_window.get();
	}

	/// Setters

	void Window::setWidth(const int width)
	{
		assert(width > 0);
		m_extent.x = width;
		glfwSetWindowSize(m_window.get(), m_extent.x, m_extent.y);
	}

	void Window::setHeight(const int height)
	{
		assert(height > 0);
		m_extent.y = height;
		glfwSetWindowSize(m_window.get(), m_extent.x, m_extent.y);
	}

	void Window::setTitle(const char * const title)
	{
		glfwSetWindowTitle(m_window.get(), title);
	}

	void Window::setCursorMode(const Window::CursorMode mode)
	{
		m_currentCursorMode = mode;
		glfwSetInputMode(m_window.get(), GLFW_CURSOR, static_cast<int>(m_currentCursorMode));
	}

	/// Callbacks

	void Window::callback_error(int error, const char * description)
	{
		Logger const log{"windowing"};
		log.error("GLFW Error: {} - {}", error, description);
	}

	void Window::callback_window_size(GLFWwindow * window, int width, int height)
	{
		auto * const self = static_cast<Window *>(glfwGetWindowUserPointer(window));
		self->m_extent.x  = width;
		self->m_extent.y  = height;
		self->m_logger.info("Window resized to {}x{}", width, height);
	}

	void Window::Deleter::operator()(GLFWwindow * ptr) const
	{
		glfwDestroyWindow(ptr);
		glfwTerminate();
	}
} // namespace gen
