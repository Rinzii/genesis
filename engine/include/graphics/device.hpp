// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"
#include "windowing/window.hpp"

#include <vector>

#include <vulkan/vulkan.hpp>

namespace gen
{

	class GraphicsDevice
	{
	public:
		GraphicsDevice(const GraphicsDevice &)			   = delete;
		GraphicsDevice & operator=(const GraphicsDevice &) = delete;
		GraphicsDevice(GraphicsDevice &&)				   = delete;
		GraphicsDevice & operator=(GraphicsDevice &&)	   = delete;

#if defined(GEN_NDEBUG)
		static constexpr bool enableValidationLayers = false;
#else
		static constexpr bool enableValidationLayers = true;
#endif

		explicit GraphicsDevice(Window & window);
		~GraphicsDevice();

	private:
		void createInstance(const std::string & appName, const std::string & engineName, const gen::u32 & apiVersion);

		vk::UniqueInstance m_instance;
	};
} // namespace gen
