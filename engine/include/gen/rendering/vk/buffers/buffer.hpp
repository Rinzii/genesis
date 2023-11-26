// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

namespace gen
{
	class Buffer
	{
	public:
		Buffer() = default;
		~Buffer() = default;

		Buffer(const Buffer &) = delete;
		Buffer(Buffer &&) = delete;
		Buffer & operator=(const Buffer &) = delete;
		Buffer & operator=(Buffer &&) = delete;
	};

} // namespace gen