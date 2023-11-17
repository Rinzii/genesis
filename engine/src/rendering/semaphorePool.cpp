// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/semaphorePool.hpp"
#include "gen/rendering/device.hpp"
#include "gen/rendering/utils/vkHelpers.hpp"

namespace gen
{
	SemaphorePool::~SemaphorePool()
	{
		reset();
		m_semaphores.clear();
	}

	vk::Semaphore SemaphorePool::requestSemaphore()
	{
		if (m_activeSemaphoreCount < m_semaphores.size()) { return m_semaphores[m_activeSemaphoreCount++].get(); }

		vk::SemaphoreCreateInfo const semaphoreCreateInfo{};

		m_semaphores.push_back(Device::self().getHandle().createSemaphoreUnique(semaphoreCreateInfo));

		return m_semaphores[m_activeSemaphoreCount++].get();
	}

	void SemaphorePool::reset()
	{
		m_activeSemaphoreCount = 0;
	}

} // namespace gen