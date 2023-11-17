// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

namespace gen
{
	class BufferPool
	{
	public:
		BufferPool() = default;

		~BufferPool() = default;

		BufferPool(const BufferPool &)			   = delete;
		BufferPool(BufferPool &&)				   = delete;
		BufferPool & operator=(const BufferPool &) = delete;
		BufferPool & operator=(BufferPool &&)	   = delete;
	};

} // namespace gen