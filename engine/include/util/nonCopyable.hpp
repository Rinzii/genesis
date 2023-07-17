// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

/*
 * \brief A helper class that makes a derived class non-copyable.
 */
class NonCopyable
{
public:
	NonCopyable() = default;
	~NonCopyable() = default;
	NonCopyable( NonCopyable&& ) = default;
	NonCopyable& operator=( NonCopyable&& ) = default;

	NonCopyable( NonCopyable const & ) = delete;
	NonCopyable& operator=( NonCopyable const & ) = delete;

};
