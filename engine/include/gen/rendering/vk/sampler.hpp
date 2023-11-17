// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vulkan/vulkan.hpp>

namespace gen
{
	class Sampler
	{
	public:
		Sampler(vk::SamplerCreateInfo & createInfo);
		~Sampler() = default;

		Sampler(const Sampler &) = delete;
		Sampler(Sampler &&) = delete;
		Sampler & operator=(const Sampler &) = delete;
		Sampler & operator=(Sampler &&) = delete;

	private:
		vk::UniqueSampler m_handle;
	};

} // namespace gen