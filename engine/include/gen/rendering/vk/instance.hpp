// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>

namespace gen
{
	class Instance2
	{
	private:
		static constexpr u32 s_minimumApiVersion = VK_API_VERSION_1_3;

	public:
		Instance2(const std::string & appName, u32 appVersion, const std::string & engineName, u32 engineVersion, u32 apiVersion = VK_API_VERSION_1_3);
		~Instance2();

		Instance2(Instance2 const &)			 = delete;
		Instance2(Instance2 &&) noexcept		 = default;
		Instance2 & operator=(Instance2 const &) = delete;
		Instance2 & operator=(Instance2 &&)		 = delete;

		vk::Instance getHandle() const { return m_handle.get(); }

	private:
		vk::UniqueInstance m_handle{nullptr};
	};
} // namespace gen