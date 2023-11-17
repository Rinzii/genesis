// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/core/monoInstance.hpp"
#include "gen/logger/log.hpp"
#include "gen/system/types.hpp"

#include <mim/vec2.hpp>
#include <vulkan/vulkan.hpp>

#include <functional>
#include <iosfwd>
#include <memory>

struct GLFWwindow;

namespace gen
{
	class RenderWindow : public MonoInstance<RenderWindow>
	{
	public:
		// Values mirrored from GLFW
		// https://www.glfw.org/docs/3.3/glfw3_8h.html#a2315b99a329ce53e6a13a9d46fd5ca88
		enum class CursorMode : int
		{
			eNormal	  = 0x00034001,
			eHidden	  = 0x00034002,
			eDisabled = 0x00034003
		};

		enum class ScreenMode : int
		{
			eWindowed = 0,
			eBorderless,
			eFullscreen
		};

		enum class VsyncMode : int
		{
			eOff = 0,
			eOn,
			eAdaptive
		};

		struct WindowProperties
		{
			WindowProperties();

			std::string title;
			ScreenMode screenMode{ScreenMode::eWindowed};
			CursorMode cursorMode{CursorMode::eNormal};
			VsyncMode vsyncMode{VsyncMode::eOff};
			mim::vec2i extent{1280, 720}; // NOLINT
			mim::vec2i position{0, 0};	  // NOLINT
			bool resizable{true};
			bool visible{true};
		};

		// DEFAULT window properties

		const static WindowProperties DEFAULT_WINDOW_PROPERTIES;

		RenderWindow(int width, int height, const char * title);

		~RenderWindow() = default;

		// Functions

		/// \brief In the event that the window has been minimized, this function will block and process events until the window is restored.
		void waitForFocus();

		[[nodiscard]] vk::SurfaceKHR createSurface(vk::Instance instance) const;

		mim::vec2<u32> resize(int width, int height);

		mim::vec2<u32> resize(const mim::vec2u & size);

		// Generic glfw events

		[[nodiscard]] bool shouldClose() const;
		void showWindow() const;
		void closeWindow() const;
		static void pollEvents();

		// Complex Getters

		[[nodiscard]] float getAspectRatio() const;
		[[nodiscard]] static float getDpiFactor();
		[[nodiscard]] float getContentScaleFactor() const;
		[[nodiscard]] static std::vector<const char *> getRequiredSurfaceExtensions();

		// Getters

		[[nodiscard]] GLFWwindow * getHandle() const { return m_handle.get(); }
		[[nodiscard]] const std::string & getTitle() const { return m_properties.title; }
		[[nodiscard]] const mim::vec2i & getExtent() const { return m_properties.extent; }
		[[nodiscard]] const WindowProperties & getProperties() const { return m_properties; }
		[[nodiscard]] const ScreenMode & getScreenMode() const { return m_properties.screenMode; }
		[[nodiscard]] const CursorMode & getCursorMode() const { return m_properties.cursorMode; }

		// Setters

		void setTitle(const std::string & title);
		void setSize(const mim::vec2u & size) const;
		void setWidth(u32 width) const;
		void setHeight(u32 height) const;
		void setScreenMode(const ScreenMode & mode);
		void setCursorMode(const CursorMode & mode);

	private:
		// Helpers
		[[nodiscard]] static RenderWindow & getWindow(GLFWwindow * window);

		// Callbacks
		static void callback_error(int error, const char * description);
		static void callback_windowCloseCallback(GLFWwindow * window);
		static void callback_windowPosCallback(GLFWwindow * window, int xpos, int ypos);
		static void callback_windowSizeCallback(GLFWwindow * window, int width, int height);
		static void callback_framebufferSizeCallback(GLFWwindow * window, int width, int height);
		// static void callback_keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
		// static void callback_mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);

		struct Deleter
		{
			void operator()(GLFWwindow * ptr) const;
		};

		std::unique_ptr<GLFWwindow, Deleter> m_handle;
		WindowProperties m_properties;
		bool m_shouldWeUpdateFramebuffer{false};

		Logger m_logger{"windowing"};
	};

} // namespace gen