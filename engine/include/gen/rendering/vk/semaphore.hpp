// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vulkan/vulkan.hpp>

#include <string>

namespace gen
{
	class Semaphore
	{
	public:
		Semaphore(const std::string & name);
		~Semaphore();

		Semaphore(Semaphore const &) = delete;
		Semaphore(Semaphore &&) noexcept;
		Semaphore & operator=(Semaphore const &) = delete;
		Semaphore & operator=(Semaphore &&)		 = delete;

	private:
		vk::UniqueSemaphore m_handle{nullptr};
		std::string m_name;
	};
} // namespace gen