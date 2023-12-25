// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/renderContext.hpp"

namespace gen
{
	RenderContext::RenderContext(
		vk::SurfaceKHR surface,
		vk::PresentModeKHR presentMode,
		const std::span<vk::PresentModeKHR> & presentModePriorityList,
		const std::span<vk::SurfaceFormatKHR> & surfaceFormatPriorityList)
	{
	}
} // namespace gen