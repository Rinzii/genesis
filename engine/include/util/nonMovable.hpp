//
// Created by Ian on 7/16/2023.
//

#pragma once

/*
 * \brief A helper class that makes a derived class non-movable.
 */

class NonMovable
{
public:
	NonMovable() = default;
	~NonMovable() = default;
	NonMovable( NonMovable const & ) = default;
	NonMovable& operator=( NonMovable const & ) = default;

	NonMovable( NonMovable&& ) = delete;
	NonMovable& operator=( NonMovable&& ) = delete;
};
