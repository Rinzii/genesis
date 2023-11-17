// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>

#include <limits>
#include <vector>

namespace gen
{
	class FencePool
	{
	public:
		FencePool() = default;

		~FencePool();

		FencePool(const FencePool &)			 = delete;
		FencePool(FencePool &&)					 = delete;
		FencePool & operator=(const FencePool &) = delete;
		FencePool & operator=(FencePool &&)		 = delete;

		vk::Fence requestFence();

		vk::Result waitForFences(u32 timeout = std::numeric_limits<u32>::max()) const; // NOLINT

		vk::Result resetFences();

	private:
		std::vector<vk::Fence> m_fences;
		u32 m_activeFenceCount{0};
	};

} // namespace gen