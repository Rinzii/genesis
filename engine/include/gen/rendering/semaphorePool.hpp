// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>

#include <vector>

namespace gen
{
	class SemaphorePool
	{
	public:
		SemaphorePool() = default;

		~SemaphorePool();

		SemaphorePool(const SemaphorePool &)			 = delete;
		SemaphorePool(SemaphorePool &&)					 = delete;
		SemaphorePool & operator=(const SemaphorePool &) = delete;
		SemaphorePool & operator=(SemaphorePool &&)		 = delete;

		vk::Semaphore requestSemaphore();

		void reset();

		u32 getActiveSemaphoreCount() const { return m_activeSemaphoreCount; }

	private:
		std::vector<vk::UniqueSemaphore> m_semaphores;

		u32 m_activeSemaphoreCount{0};
	};
} // namespace gen