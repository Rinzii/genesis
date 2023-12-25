// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <cassert>
#include <cstdio>
#include <cstring>

#include <string>
#include <system_error>
#include <vector>

#include <vulkan/vulkan.hpp>

// #include "gen/rendering/utils/vkHelpers.hpp"

namespace vk::utils
{
	struct Error
	{
		std::error_code type;
		vk::Result vkResult = vk::Result::eSuccess; // Optional value in the event a vulkan call fails.
	};

	template <typename T>
	class Result
	{
		Result(const T & value) noexcept : m_value{value}, m_init{true} {}
		Result(T && value) noexcept : m_value{std::move(value)}, m_init{true} {}

		Result(Error error) noexcept : m_error{error}, m_init{false} {}

		Result(std::error_code error_code, vk::Result result = vk::Result::eSuccess) noexcept : m_error{error_code, result}, m_init{false} {}

		~Result() noexcept { destroy(); }
		Result(Result const & expected) noexcept : m_init(expected.m_init)
		{
			if (m_init)
				new (&m_value) T{expected.m_value};
			else
				m_error = expected.m_error;
		}
		Result & operator=(Result const & result) noexcept
		{
			m_init = result.m_init;
			if (m_init)
				new (&m_value) T{result.m_value};
			else
				m_error = result.m_error;
			return *this;
		}
		Result(Result && expected) noexcept : m_init(expected.m_init)
		{
			if (m_init)
				new (&m_value) T{std::move(expected.m_value)};
			else
				m_error = std::move(expected.m_error);
			expected.destroy();
		}
		Result & operator=(Result && result) noexcept
		{
			m_init = result.m_init;
			if (m_init)
				new (&m_value) T{std::move(result.m_value)};
			else
				m_error = std::move(result.m_error);
			return *this;
		}
		Result & operator=(const T & expect) noexcept
		{
			destroy();
			m_init = true;
			new (&m_value) T{expect};
			return *this;
		}
		Result & operator=(T && expect) noexcept
		{
			destroy();
			m_init = true;
			new (&m_value) T{std::move(expect)};
			return *this;
		}
		Result & operator=(const Error & error) noexcept
		{
			destroy();
			m_init	= false;
			m_error = error;
			return *this;
		}
		Result & operator=(Error && error) noexcept
		{
			destroy();
			m_init	= false;
			m_error = error;
			return *this;
		}
		const T * operator->() const noexcept
		{
			assert(m_init);
			return &m_value;
		}
		T * operator->() noexcept
		{
			assert(m_init);
			return &m_value;
		}
		const T & operator*() const & noexcept
		{
			assert(m_init);
			return m_value;
		}
		T & operator*() & noexcept
		{
			assert(m_init);
			return m_value;
		}
		T && operator*() && noexcept
		{
			assert(m_init);
			return std::move(m_value);
		}
		const T & value() const & noexcept
		{
			assert(m_init);
			return m_value;
		}
		T & value() & noexcept
		{
			assert(m_init);
			return m_value;
		}
		const T && value() const && noexcept
		{
			assert(m_init);
			return std::move(m_value);
		}
		T && value() && noexcept
		{
			assert(m_init);
			return std::move(m_value);
		}

		// std::error_code associated with the error
		std::error_code error() const
		{
			assert(!m_init);
			return m_error.type;
		}
		// optional VkResult that could of been produced due to the error
		vk::Result vk_result() const
		{
			assert(!m_init);
			return m_error.result;
		}
		// Returns the struct that holds the std::error_code and VkResult
		Error full_error() const
		{
			assert(!m_init);
			return m_error;
		}

		// check if the result has an error that matches a specific error case
		template <typename E>
		bool matches_error(E error_enum_value) const
		{
			return !m_init && static_cast<E>(m_error.type.value()) == error_enum_value;
		}

		bool has_value() const { return m_init; }
		explicit operator bool() const { return m_init; }

	private:
		void destroy()
		{
			if (m_init) m_value.~T();
		}
		union
		{
			T m_value;
			Error m_error;
		};
		bool m_init;
	};
} // namespace vk::utils