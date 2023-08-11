#include "inputs/pcPeripherals.hpp"

#include "logger/log.hpp"
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>

namespace gen
{
	namespace
	{
		const std::map<int, int> g_keyVectorIndexMappings =
		{
			{ static_cast<int>(KeyCode::SPACE), 32 },
			{ static_cast<int>(KeyCode::APOSTROPHE), 39 },
			{ static_cast<int>(KeyCode::COMMA), 44 },
			{ static_cast<int>(KeyCode::MINUS), 45 },
			{ static_cast<int>(KeyCode::PERIOD), 46 },
			{ static_cast<int>(KeyCode::SLASH), 47 },
			{ static_cast<int>(KeyCode::D0), 48 },
			{ static_cast<int>(KeyCode::D1), 49 },
			{ static_cast<int>(KeyCode::D2), 50 },
			{ static_cast<int>(KeyCode::D3), 51 },
			{ static_cast<int>(KeyCode::D4), 52 },
			{ static_cast<int>(KeyCode::D5), 53 },
			{ static_cast<int>(KeyCode::D6), 54 },
			{ static_cast<int>(KeyCode::D7), 55 },
			{ static_cast<int>(KeyCode::D8), 56 },
			{ static_cast<int>(KeyCode::D9), 57 },
			{ static_cast<int>(KeyCode::SEMICOLON), 59 },
			{ static_cast<int>(KeyCode::EQUAL), 61 },
			{ static_cast<int>(KeyCode::A), 65 },
			{ static_cast<int>(KeyCode::B), 66 },
			{ static_cast<int>(KeyCode::C), 67 },
			{ static_cast<int>(KeyCode::D), 68 },
			{ static_cast<int>(KeyCode::E), 69 },
			{ static_cast<int>(KeyCode::F), 70 },
			{ static_cast<int>(KeyCode::G), 71 },
			{ static_cast<int>(KeyCode::H), 72 },
			{ static_cast<int>(KeyCode::I), 73 },
			{ static_cast<int>(KeyCode::J), 74 },
			{ static_cast<int>(KeyCode::K), 75 },
			{ static_cast<int>(KeyCode::L), 76 },
			{ static_cast<int>(KeyCode::M), 77 },
			{ static_cast<int>(KeyCode::N), 78 },
			{ static_cast<int>(KeyCode::O), 79 },
			{ static_cast<int>(KeyCode::P), 80 },
			{ static_cast<int>(KeyCode::Q), 81 },
			{ static_cast<int>(KeyCode::R), 82 },
			{ static_cast<int>(KeyCode::S), 83 },
			{ static_cast<int>(KeyCode::T), 84 },
			{ static_cast<int>(KeyCode::U), 85 },
			{ static_cast<int>(KeyCode::V), 86 },
			{ static_cast<int>(KeyCode::W), 87 },
			{ static_cast<int>(KeyCode::X), 88 },
			{ static_cast<int>(KeyCode::Y), 89 },
			{ static_cast<int>(KeyCode::Z), 90 },
			{ static_cast<int>(KeyCode::LEFTBRACKET), 91 },
			{ static_cast<int>(KeyCode::BACKSLASH), 92 },
			{ static_cast<int>(KeyCode::RIGHTBRACKET), 93 },
			{ static_cast<int>(KeyCode::GRAVEACCENT), 96 },
			{ static_cast<int>(KeyCode::WORLD1), 161 },
			{ static_cast<int>(KeyCode::WORLD2), 162 },
			{ static_cast<int>(KeyCode::ESCAPE), 256 },
			{ static_cast<int>(KeyCode::ENTER), 257 },
			{ static_cast<int>(KeyCode::TAB), 258 },
			{ static_cast<int>(KeyCode::BACKSPACE), 259 },
			{ static_cast<int>(KeyCode::INSERT), 260 },
			{ static_cast<int>(KeyCode::DELETE), 261 },
			{ static_cast<int>(KeyCode::RIGHT), 262 },
			{ static_cast<int>(KeyCode::LEFT), 263 },
			{ static_cast<int>(KeyCode::DOWN), 264 },
			{ static_cast<int>(KeyCode::UP), 265 },
			{ static_cast<int>(KeyCode::PAGEUP), 266 },
			{ static_cast<int>(KeyCode::PAGEDOWN), 267 },
			{ static_cast<int>(KeyCode::HOME), 268 },
			{ static_cast<int>(KeyCode::END), 269 },
			{ static_cast<int>(KeyCode::CAPSLOCK), 280 },
			{ static_cast<int>(KeyCode::SCROLLLOCK), 281 },
			{ static_cast<int>(KeyCode::NUMLOCK), 282 },
			{ static_cast<int>(KeyCode::PRINTSCREEN), 283 },
			{ static_cast<int>(KeyCode::PAUSE), 284 },
			{ static_cast<int>(KeyCode::F1), 290 },
			{ static_cast<int>(KeyCode::F2), 291 },
			{ static_cast<int>(KeyCode::F3), 292 },
			{ static_cast<int>(KeyCode::F4), 293 },
			{ static_cast<int>(KeyCode::F5), 294 },
			{ static_cast<int>(KeyCode::F6), 295 },
			{ static_cast<int>(KeyCode::F7), 296 },
			{ static_cast<int>(KeyCode::F8), 297 },
			{ static_cast<int>(KeyCode::F9), 298 },
			{ static_cast<int>(KeyCode::F10), 299 },
			{ static_cast<int>(KeyCode::F11), 300 },
			{ static_cast<int>(KeyCode::F12), 301 },
			{ static_cast<int>(KeyCode::F13), 302 },
			{ static_cast<int>(KeyCode::F14), 303 },
			{ static_cast<int>(KeyCode::F15), 304 },
			{ static_cast<int>(KeyCode::F16), 305 },
			{ static_cast<int>(KeyCode::F17), 306 },
			{ static_cast<int>(KeyCode::F18), 307 },
			{ static_cast<int>(KeyCode::F19), 308 },
			{ static_cast<int>(KeyCode::F20), 309 },
			{ static_cast<int>(KeyCode::F21), 310 },
			{ static_cast<int>(KeyCode::F22), 311 },
			{ static_cast<int>(KeyCode::F23), 312 },
			{ static_cast<int>(KeyCode::F24), 313 },
			{ static_cast<int>(KeyCode::F25), 314 },
			{ static_cast<int>(KeyCode::KP0), 320 },
			{ static_cast<int>(KeyCode::KP1), 321 },
			{ static_cast<int>(KeyCode::KP2), 322 },
			{ static_cast<int>(KeyCode::KP3), 323 },
			{ static_cast<int>(KeyCode::KP4), 324 },
			{ static_cast<int>(KeyCode::KP5), 325 },
			{ static_cast<int>(KeyCode::KP6), 326 },
			{ static_cast<int>(KeyCode::KP7), 327 },
			{ static_cast<int>(KeyCode::KP8), 328 },
			{ static_cast<int>(KeyCode::KP9), 329 },
			{ static_cast<int>(KeyCode::KPDECIMAL), 330 },
			{ static_cast<int>(KeyCode::KPDIVIDE), 331 },
			{ static_cast<int>(KeyCode::KPMULTIPLY), 332 },
			{ static_cast<int>(KeyCode::KPSUBSTRACT), 333 },
			{ static_cast<int>(KeyCode::KPADD), 334 },
			{ static_cast<int>(KeyCode::KPENTER), 335 },
			{ static_cast<int>(KeyCode::KPEQUAL), 336 },
			{ static_cast<int>(KeyCode::LEFTSHIFT), 340 },
			{ static_cast<int>(KeyCode::LEFTCONTROL), 341 },
			{ static_cast<int>(KeyCode::LEFTALT), 342 },
			{ static_cast<int>(KeyCode::LEFTSUPER), 343 },
			{ static_cast<int>(KeyCode::RIGHTSHIFT), 344 },
			{ static_cast<int>(KeyCode::RIGHTCONTROL), 345 },
			{ static_cast<int>(KeyCode::RIGHTALT), 346 },
			{ static_cast<int>(KeyCode::RIGHTSUPER), 347 },
			{ static_cast<int>(KeyCode::MENU), 348}
		};
	}

	Keyboard::Keyboard()
	{
		m_keys.reserve(g_keyVectorIndexMappings.size());
	}

	void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		const int resultIndex = glfwMacroToVectorIndex(key);

		if (resultIndex != -1)
		{
			m_keys[resultIndex] = action;
		}
		std::cout << glfwGetKeyName(key, scancode);

		logger::debug("Input", "Key isn't supported");
	}

	int Keyboard::glfwMacroToVectorIndex(int key)
	{
		const auto it = g_keyVectorIndexMappings.find(key);

		if (it != g_keyVectorIndexMappings.end())
		{
			return it->second;
		}

		return -1;
	}

	PCPeripherals::PCPeripherals()
	{
	}
}
