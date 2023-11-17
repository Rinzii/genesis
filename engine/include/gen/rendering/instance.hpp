// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/core/monoInstance.hpp"
#include "gen/logger/log.hpp"
#include "gen/system/types.hpp"

#include <iosfwd>
#include <memory>
#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace gen
{
	class Gpu;

	class Instance : public MonoInstance<Instance>
	{
	public:
		Instance(const std::string & appName, u32 appVersion, const std::string & engineName = "Genesis Engine", u32 apiVersion = VK_API_VERSION_1_3);

		explicit Instance(vk::Instance instance);

		explicit Instance(vk::UniqueInstance instance);

		~Instance() = default;

		Instance(const Instance &)			   = delete;
		Instance & operator=(const Instance &) = delete;

		Instance(Instance &&)			  = delete;
		Instance & operator=(Instance &&) = delete;

		// Getters

		[[nodiscard]] vk::Instance getHandle() const { return m_handle.get(); }

	private:
		vk::UniqueInstance m_handle;

		Logger m_logger{"vulkan"};
	};

} // namespace gen