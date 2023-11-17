// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

namespace gen
{
	class ImageView
	{
	public:
		ImageView() = default;
		~ImageView() = default;

		ImageView(const ImageView &) = delete;
		ImageView(ImageView &&) = delete;
		ImageView & operator=(const ImageView &) = delete;
		ImageView & operator=(ImageView &&) = delete;
	};

} // namespace gen