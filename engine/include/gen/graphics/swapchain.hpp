// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "device.hpp"
#include "gen/core.hpp"
#include "gen/core/monoInstance.hpp"
#include "gen/windowing/window.hpp"

// external
#include <vulkan/vulkan.hpp>

// std
#include <vector>

namespace gen
{
	namespace
	{
		struct ImageBarrier
		{
			vk::ImageMemoryBarrier2 m_barrier{};

			explicit ImageBarrier(vk::Image image, u32 mipLevels = 1, u32 arrayLayers = 1);

			ImageBarrier & setFullBarrier(vk::ImageLayout src, vk::ImageLayout dst);

			ImageBarrier & setUndefToOptimal(bool depth);

			ImageBarrier & setOptimalToPresent();

			void transition(vk::CommandBuffer cmd);

			static void transition(vk::CommandBuffer cmd, std::span<vk::ImageMemoryBarrier2 const> barriers);
		};
	} // namespace

	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities{};
		std::vector<vk::SurfaceFormatKHR> availableFormats{};
		vk::SurfaceFormatKHR selectedFormat{};
		std::vector<vk::PresentModeKHR> availablePresentModes{};
		vk::PresentModeKHR selectedPresentMode{};
	};

	class Swapchain
	{
	public:
		static constexpr std::size_t bufferingCount_v{2};

		explicit Swapchain(const Window & window);
		~Swapchain();

		Swapchain(Swapchain const &)			 = delete;
		Swapchain(Swapchain &&)					 = delete;
		Swapchain & operator=(Swapchain const &) = delete;
		Swapchain & operator=(Swapchain &&)		 = delete;

		/// Getters
		GEN_NODISCARD vk::SwapchainKHR const & getSwapChain() const { return m_swapChain.get(); }
		GEN_NODISCARD vk::Extent2D const & getExtent() const { return m_swapChainInfo.imageExtent; }
		GEN_NODISCARD vk::Format const & getFormat() const { return m_swapChainInfo.imageFormat; }
		GEN_NODISCARD std::vector<vk::Image> const & getImages() const { return m_swapChainImages; }
		GEN_NODISCARD std::vector<vk::UniqueImageView> const & getImageViews() const { return m_swapChainImageViews; }

	private:
		void createSwapChain(const Window & window, const Device & device);
		void createImageViews(const Device & device);
		void createSyncObjects(const Device & device);

		void recreateSwapChain(const Window & window, const Device & device);

		/// Helpers

		static SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface);
		static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> & availableFormats);
		static vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> & availablePresentModes, vk::PresentModeKHR preferredMode);
		static vk::Extent2D chooseSwapExtent(const SwapChainSupportDetails & capabilities, const Window & window);

		SwapChainSupportDetails m_swapChainSupport{};

		vk::SwapchainCreateInfoKHR m_swapChainInfo{};
		vk::UniqueSwapchainKHR m_swapChain{};
		vk::UniqueSwapchainKHR m_oldSwapChain{};
		std::vector<vk::Image> m_swapChainImages{};

		std::vector<vk::UniqueImageView> m_swapChainImageViews{};

		vk::UniqueCommandPool m_commandPool{};

		template <typename T>
		using Buffered = std::array<T, bufferingCount_v>;

		struct Frame
		{
			struct Sync
			{
				vk::CommandBuffer cmd{};
				vk::UniqueSemaphore image{};
				vk::UniqueSemaphore render{};
				vk::UniqueFence inFlight{};
			};

			Buffered<Sync> sync{};
			std::size_t currentFrame{0};
		} m_frames{};

		Logger m_logger{"graphics"};
	};
} // namespace gen