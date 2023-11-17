// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/fencePool.hpp"
#include "gen/rendering/device.hpp"
#include "gen/rendering/utils/vkHelpers.hpp"

namespace gen
{

	FencePool::~FencePool()
	{
		waitForFences();
		resetFences();

		for (auto & fence : m_fences) { Device::self().getHandle().destroyFence(fence); }

		m_fences.clear();
	}

	vk::Fence FencePool::requestFence()
	{
		// Double check that a fence is available
		if (m_activeFenceCount < m_fences.size()) { return m_fences.at(m_activeFenceCount++); }

		// If not, create a new one
		vk::FenceCreateInfo const fenceCreateInfo{};

		m_fences.push_back(Device::self().getHandle().createFence(fenceCreateInfo));

		m_activeFenceCount++;

		return m_fences.back();
	}

	vk::Result FencePool::waitForFences(u32 timeout) const
	{
		if (m_activeFenceCount < 1 || m_fences.empty()) { return vk::Result::eSuccess; }

		return Device::self().getHandle().waitForFences(m_activeFenceCount, m_fences.data(), vk::True, timeout);
	}

	vk::Result FencePool::resetFences()
	{
		if (m_activeFenceCount < 1 || m_fences.empty()) { return vk::Result::eSuccess; }

		auto result = Device::self().getHandle().resetFences(m_activeFenceCount, m_fences.data());

		if (result != vk::Result::eSuccess) { return result; }

		m_activeFenceCount = 0;

		return vk::Result::eSuccess;
	}
} // namespace gen