// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"
#include "gen/core/monoInstance.hpp"
#include "gen/logger/log.hpp"

#include <vulkan/vulkan.hpp>

namespace gen
{
	class Instance : public MonoInstance<Instance>
	{
	private:
		static constexpr u32 s_minimumApiVersion = VK_API_VERSION_1_3;

	public:
		Instance(const std::string & appName, u32 appVersion, const std::string & engineName, u32 engineVersion, u32 apiVersion = VK_API_VERSION_1_3);
		~Instance();


		vk::Instance getHandle() const { return m_handle.get(); }

	private:
		vk::UniqueInstance m_handle{nullptr};

		Logger m_logger{"vulkan"};
	};
} // namespace gen