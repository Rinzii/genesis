// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/renderWindow.hpp"

#include "gen/rendering/vk/instance.hpp"

#include <utility>

#include <GLFW/glfw3.h>

#include "gen/application.hpp"

namespace gen
{
	RenderWindow::WindowProperties::WindowProperties() = default;

	RenderWindow::RenderWindow(int width, int height, const char * title)
	{
#if defined(VK_USE_PLATFORM_XLIB_KHR)
		glfwInitHint(GLFW_X11_XCB_VULKAN_SURFACE, GLFW_FALSE);
#endif

		if (glfwInit() != GLFW_TRUE || glfwVulkanSupported() != GLFW_TRUE) { throw std::runtime_error("Failed to initialize GLFW"); }

		glfwSetErrorCallback(callback_error); // Ensure we have an error callback early.

		// Set window hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, m_properties.visible ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, m_properties.resizable ? GLFW_TRUE : GLFW_FALSE);

		// TODO: This will likely need more work later but is fine for now.

		switch (m_properties.screenMode)
		{
		case RenderWindow::ScreenMode::eBorderless:
		{
			auto * monitor		   = glfwGetPrimaryMonitor();
			const auto * videoMode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
			m_handle = std::unique_ptr<GLFWwindow, Deleter>(glfwCreateWindow(videoMode->width, videoMode->height, title, monitor, nullptr));
			break;
		}

		case RenderWindow::ScreenMode::eFullscreen:
		{
			auto * monitor		   = glfwGetPrimaryMonitor();
			const auto * videoMode = glfwGetVideoMode(monitor);
			m_handle			   = std::unique_ptr<GLFWwindow, Deleter>(glfwCreateWindow(videoMode->width, videoMode->height, title, monitor, nullptr));
			break;
		}

		default:
			m_handle = std::unique_ptr<GLFWwindow, Deleter>(glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr));
			break;
		}

		// Check if window was created
		if (!m_handle) { throw std::runtime_error("Failed to create GLFW window"); }

		// Set modes
		glfwSetInputMode(m_handle.get(), GLFW_CURSOR, static_cast<int>(m_properties.cursorMode));
		glfwSetInputMode(m_handle.get(), GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwSetInputMode(m_handle.get(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

		// Set all glfw callbacks
		glfwSetWindowCloseCallback(m_handle.get(), callback_windowCloseCallback);
		glfwSetWindowPosCallback(m_handle.get(), callback_windowPosCallback);
		glfwSetWindowSizeCallback(m_handle.get(), callback_windowSizeCallback);
		glfwSetFramebufferSizeCallback(m_handle.get(), callback_framebufferSizeCallback);
		// glfwSetKeyCallback(m_handle.get(), callback_keyCallback);
		// glfwSetMouseButtonCallback(m_handle.get(), callback_mouseButtonCallback);

		// Make sure all of our values are correct internally after we finish window creation.
		glfwGetWindowSize(m_handle.get(), &m_properties.extent.x, &m_properties.extent.y);
		glfwGetWindowPos(m_handle.get(), &m_properties.position.x, &m_properties.position.y);
	}

	vk::SurfaceKHR RenderWindow::createSurface(vk::Instance instance) const
	{
		if (instance == nullptr) { return nullptr; }

		VkSurfaceKHR surface{};
		VkResult const err = glfwCreateWindowSurface(instance, m_handle.get(), nullptr, &surface);

		if (err != VK_SUCCESS) { return nullptr; }

		return {surface};
	}

	void RenderWindow::waitForFocus()
	{
		mim::vec2i currentExtent = {0, 0};

		do {
			glfwWaitEvents();
			glfwGetFramebufferSize(m_handle.get(), &currentExtent.x, &currentExtent.y);
		} while (currentExtent.x == 0 || currentExtent.y == 0);

		m_properties.extent = currentExtent;
	}

	bool RenderWindow::shouldClose() const
	{
		return glfwWindowShouldClose(m_handle.get()) != 0;
	}

	void RenderWindow::showWindow() const
	{
		glfwShowWindow(m_handle.get());
	}

	void RenderWindow::closeWindow() const
	{
		glfwSetWindowShouldClose(m_handle.get(), GLFW_TRUE);
	}

	void RenderWindow::pollEvents()
	{
		glfwPollEvents();
	}

	float RenderWindow::getAspectRatio() const
	{
		return static_cast<float>(m_properties.extent.x) / static_cast<float>(m_properties.extent.y);
	}

	/// \brief Calculates the dpi factor using the density from GLFW physical size
	/// <a href="https://www.glfw.org/docs/latest/monitor_guide.html#monitor_size">GLFW docs for dpi</a>
	float RenderWindow::getDpiFactor()
	{
		auto * const monitor	= glfwGetPrimaryMonitor();
		const auto * const mode = glfwGetVideoMode(monitor);

		mim::vec2i physicalSizeMM;
		glfwGetMonitorPhysicalSize(monitor, &physicalSizeMM.x, &physicalSizeMM.y);

		// This is suggested by the glfw monitor guide.
		static const float inchToMm			 = 25.0F;
		static const float windowBaseDensity = 96.0F;

		auto const dpi	   = static_cast<u32>(static_cast<float>(mode->width) / (static_cast<float>(physicalSizeMM.x) / inchToMm));
		auto const density = static_cast<float>(dpi) / windowBaseDensity;
		return density;
	}

	float RenderWindow::getContentScaleFactor() const
	{
		mim::vec2i frameBufferSize;
		glfwGetFramebufferSize(m_handle.get(), &frameBufferSize.x, &frameBufferSize.y);
		mim::vec2i windowSize;
		glfwGetWindowSize(m_handle.get(), &windowSize.x, &windowSize.y);

		return static_cast<float>(frameBufferSize.x) / static_cast<float>(windowSize.x);
	}

	std::vector<const char *> RenderWindow::getRequiredSurfaceExtensions()
	{
		u32 glfwExtensionCount		 = 0;
		const char ** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		return {glfwExtensions, glfwExtensions + glfwExtensionCount}; // NOLINT
	}

	mim::vec2<u32> RenderWindow::resize(int width, int height)
	{
		if (m_properties.resizable) { m_properties.extent = {width, height}; }

		return mim::vec2<u32>(m_properties.extent.x, m_properties.extent.y);
	}

	mim::vec2<u32> RenderWindow::resize(const mim::vec2u & size)
	{
		return resize(static_cast<int>(size.x), static_cast<int>(size.y));
	}

	void RenderWindow::setTitle(const std::string & title)
	{
		m_properties.title = title;
		glfwSetWindowTitle(m_handle.get(), m_properties.title.c_str());
	}

	void RenderWindow::setSize(const mim::vec2u & size) const
	{
		glfwSetWindowSize(m_handle.get(), static_cast<int>(size.x), static_cast<int>(size.y));
	}

	void RenderWindow::setWidth(u32 width) const
	{
		glfwSetWindowSize(m_handle.get(), static_cast<int>(width), m_properties.extent.y);
	}

	void RenderWindow::setHeight(u32 height) const
	{
		glfwSetWindowSize(m_handle.get(), m_properties.extent.x, static_cast<int>(height));
	}

	void RenderWindow::setScreenMode(const RenderWindow::ScreenMode & mode)
	{
		if (m_properties.screenMode == mode) { return; }

		m_properties.screenMode = mode;

		// Backup current window position and size.
		glfwGetWindowPos(m_handle.get(), &m_properties.position.x, &m_properties.position.y);
		glfwGetWindowSize(m_handle.get(), &m_properties.extent.x, &m_properties.extent.y);

		switch (m_properties.screenMode)
		{
		case RenderWindow::ScreenMode::eWindowed:
			glfwSetWindowMonitor(m_handle.get(), nullptr, m_properties.position.x, m_properties.position.y, m_properties.extent.x, m_properties.extent.y, 0);
			break;
		case RenderWindow::ScreenMode::eBorderless:
			glfwSetWindowAttrib(m_handle.get(), GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowAttrib(m_handle.get(), GLFW_RESIZABLE, GLFW_FALSE);
			glfwSetWindowAttrib(m_handle.get(), GLFW_FLOATING, GLFW_TRUE);
			glfwSetWindowMonitor(m_handle.get(), nullptr, 0, 0, m_properties.extent.x, m_properties.extent.y, 0);
			break;
		case RenderWindow::ScreenMode::eFullscreen:
			glfwSetWindowAttrib(m_handle.get(), GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowAttrib(m_handle.get(), GLFW_RESIZABLE, GLFW_FALSE);
			auto * const monitor		 = glfwGetPrimaryMonitor();
			const auto * const videoMode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(m_handle.get(), monitor, 0, 0, videoMode->width, videoMode->height, 0);
			break;
		}
	}

	void RenderWindow::setCursorMode(const RenderWindow::CursorMode & mode)
	{
		if (m_properties.cursorMode == mode) { return; }
		m_properties.cursorMode = mode;
		glfwSetInputMode(m_handle.get(), GLFW_CURSOR, static_cast<int>(m_properties.cursorMode));
	}

	// Helpers

	RenderWindow & RenderWindow::getWindow(GLFWwindow * window)
	{
		auto * const self = static_cast<Application *>(glfwGetWindowUserPointer(window));
		assert(self != nullptr);
		return self->getWindow();
	}

	// Callbacks

	void RenderWindow::callback_error(int error, const char * description)
	{
		Logger const log{"windowing"};
		log.error("GLFW Error: {} - {}", error, description);
	}

	void RenderWindow::callback_windowCloseCallback(GLFWwindow * window)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void RenderWindow::callback_windowPosCallback(GLFWwindow * window, int x, int y)
	{
		auto & self				   = getWindow(window);
		self.m_properties.position = {x, y};
	}

	void RenderWindow::callback_windowSizeCallback(GLFWwindow * window, int width, int height)
	{
		auto & self = getWindow(window);
		self.resize(width, height);
	}

	void RenderWindow::callback_framebufferSizeCallback(GLFWwindow * window, int width, int height)
	{
		auto & self = getWindow(window);
		self.resize(width, height);
	}

	void RenderWindow::Deleter::operator()(GLFWwindow * ptr) const
	{
		glfwDestroyWindow(ptr);
		glfwTerminate();
	}

} // namespace gen