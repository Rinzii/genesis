// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

#include <string>

namespace gen
{
	class Image
	{
	public:
		Image(vk::Image image, vk::Format format, vk::Extent2D extent, vk::ImageUsageFlags usage, vk::ImageAspectFlags aspectFlags);
		~Image();

		Image(Image const &)			 = delete;
		Image(Image &&) noexcept		 = default;
		Image & operator=(Image const &) = delete;
		Image & operator=(Image &&)		 = delete;

		vk::Image getHandle() const { return m_image.get(); }

	private:
		VmaAllocation m_allocation{nullptr};
		VmaAllocationInfo m_allocationInfo{};
		vk::UniqueImage m_image{nullptr};
		vk::UniqueImageView m_imageView{nullptr};
		vk::Format m_format{vk::Format::eUndefined};

		std::string m_name;
	};
} // namespace gen