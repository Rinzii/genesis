// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include "gen/rendering/vk/buffer.hpp"
#include "gen/rendering/vk/imageView.hpp"

#include <vulkan/vulkan.hpp>

#include <map>

namespace gen
{
	template <class T>
	using BindingMap = std::map<std::uint32_t, std::map<std::uint32_t, T>>;

	struct ResourceInfo
	{
		bool dirty {false};
		Buffer * buffer {nullptr};
		vk::DeviceSize offset {0};
		vk::DeviceSize range {0};
		ImageView * imageView {nullptr};
	};

	class ResourceSet
	{
	public:
		void reset();

		bool isDirty();

		void clearDirty();

		void clearDirty(u32 binding, u32 arrayElement);

		void bindBuffer(Buffer & buffer, vk::DeviceSize offset, vk::DeviceSize range, u32 binding, u32 arrayElement);

		void bindImage(ImageView & imageView, u32 binding, u32 arrayElement);

		void bindInputAttachment(ImageView & imageView, u32 binding, u32 arrayElement);

		const BindingMap<Resource
	};

	class ResourceBindingState
	{
	};
}