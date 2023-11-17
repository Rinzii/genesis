// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vulkan/vulkan.hpp>

namespace gen
{
	class Fence
	{
	public:
		Fence(const std::string & name, bool signaled);
		~Fence();

		Fence(Fence const &)			 = delete;
		Fence(Fence &&) noexcept		 = default;
		Fence & operator=(Fence const &) = delete;
		Fence & operator=(Fence &&)		 = delete;

		vk::Fence getHandle() const { return m_handle.get(); }

	private:
		vk::UniqueFence m_handle{nullptr};
		std::string m_name;
	};
} // namespace gen