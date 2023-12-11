// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/core/monoInstance.hpp"
#include "gen/system/types.hpp"

#include "gen/rendering/device.hpp"
#include "gen/rendering/instance.hpp"
#include "gen/rendering/renderFrame.hpp"
#include "gen/rendering/swapchain.hpp"

#include <memory>

namespace gen
{

	class Renderer : public MonoInstance<Renderer>
	{
	public:
		Renderer(const char * appName, u32 appVersion, const char * engineName, const u32 & apiVersion);
		~Renderer() = default;

		Renderer(const Renderer &)			   = delete;
		Renderer(Renderer &&)				   = delete;
		Renderer & operator=(const Renderer &) = delete;
		Renderer & operator=(Renderer &&)	   = delete;

		void render();

	private:
		void prepare();

		void updateSwapchain(const vk::Extent2D & extent);

		void updateSwapchain(const u32 imageCount);

		void updateSwapchain(const std::set<vk::ImageUsageFlagBits> & imageUsageFlags);

		void updateSwapchain(const vk::Extent2D & extent, const vk::SurfaceTransformFlagBitsKHR transform);

		void recreateFrame();

		void recreateSwapchain();

	protected:
		vk::Extent2D m_surfaceExtent;

	private:
		std::unique_ptr<Instance> m_instance;
		std::unique_ptr<Device> m_device;
		std::unique_ptr<Swapchain> m_swapchain;
		SwapchainProperties m_swapchainProperties;

		std::vector<std::unique_ptr<Frame>> m_frames;

		u32 m_activeFrameIndex{0};
		bool m_frameCurrentlyActive{false};

		bool m_prepared{false};
	};

} // namespace gen