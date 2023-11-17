// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"
#include "gen/rendering/vk/fence.hpp"
#include "gen/rendering/vk/buffer.hpp"
#include "gen/rendering/vk/image.hpp"
#include "gen/rendering/vk/imageView.hpp"
#include "gen/rendering/utils/vulkanStructs.hpp"


#include <vulkan/vulkan.hpp>

#include <memory>
#include <string>

namespace gen
{
	class CommandPool;
	class DescriptorSet;
	class Pipeline;
	class PipelineLayout;
	class PipelineState;
	class RenderTarget;
	struct LightingState;

	class CommandBuffer
	{
	public:
		enum class ResetMode
		{
			ResetPool,
			ResetIndividually,
			AlwaysAllocate
		};

		CommandBuffer(CommandPool & cmdPool, vk::CommandBufferLevel level, std::string name);
		~CommandBuffer();

		CommandBuffer(CommandBuffer const &)				 = delete;
		CommandBuffer(CommandBuffer &&) noexcept;
		CommandBuffer & operator=(CommandBuffer const &)	 = delete;
		CommandBuffer & operator=(CommandBuffer &&) noexcept = default;

		void flush(vk::PipelineBindPoint bindPoint);

		vk::Result begin(vk::CommandBufferUsageFlags flags, CommandBuffer * primaryCmdBuffer = nullptr);

		vk::Result end();

		void clear(vk::ClearAttachment info, vk::ClearRect rect);

		void beginRendering(RenderTarget & renderTarget);

		void endRendering();

		void bindPipelineLayout(PipelineLayout & pipeline);

		void bindBuffer(Buffer & buffer, vk::DeviceSize offset, vk::DeviceSize range, u32 set, u32 binding, u32 arrayElement);

		void bindImage(ImageView & imageView, u32 set, u32 binding, u32 arrayElement);

		void bindInputAttachment(ImageView & imageView, u32 set, u32 binding, u32 arrayElement);

		void bindVertexBuffers(u32 firstBinding, std::vector<Buffer> & buffers, std::vector<vk::DeviceSize> & offsets);

		void bindIndexBuffer(Buffer & buffer, vk::DeviceSize offset, vk::IndexType indexType);


		void setViewport(u32 firstViewport, const std::vector<vk::Viewport> & viewports);

		void setScissor(u32 firstScissor, const std::vector<vk::Rect2D> & scissors);

		void setLineWidth(float lineWidth);

		void setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);

		void setBlendConstants(const std::array<float, 4> & blendConstants);

		void setDepthBounds(float minDepthBounds, float maxDepthBounds);

		void draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);

		void drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, u32 vertexOffset, u32 firstInstance);

		void drawIndirect(Buffer & buffer, vk::DeviceSize offset, u32 drawCount, u32 stride);

		void dispatch(u32 groupCountX, u32 groupCountY, u32 groupCountZ);

		void dispatchIndirect(Buffer & buffer, vk::DeviceSize offset);

		void updateBuffer(Buffer & buffer, vk::DeviceSize offset, const std::vector<u8> & data);

		void blitImage(
			Image & srcImage,
			vk::ImageLayout srcImageLayout,
			Image & dstImage,
			vk::ImageLayout dstImageLayout,
			const std::vector<vk::ImageBlit> & regions,
			vk::Filter filter);

		void copyBuffer(Buffer & srcBuffer, Buffer & dstBuffer, const std::vector<vk::BufferCopy> & regions);

		void copyImage(
			Image & srcImage,
			vk::ImageLayout srcImageLayout,
			Image & dstImage,
			vk::ImageLayout dstImageLayout,
			const std::vector<vk::ImageCopy> & regions);

		void copyBufferToImage(
			Buffer & srcBuffer,
			Image & dstImage,
			vk::ImageLayout dstImageLayout,
			const std::vector<vk::BufferImageCopy> & regions);

		void copyImageToBuffer(
			Image & srcImage,
			vk::ImageLayout srcImageLayout,
			Buffer & dstBuffer,
			const std::vector<vk::BufferImageCopy> & regions);

		void imageMemoryBarrier(
			Image & image,
			vk::types::ImageMemoryBarrier & barrier) const;

		void bufferMemoryBarrier(Buffer & buffer, vk::DeviceSize offset, vk::DeviceSize size, vk::types::BufferMemoryBarrier & barrier) const;

		void setUpdateAfterBind(bool updateAfterBind);

		void resetQueryPool(vk::QueryPool queryPool, u32 firstQuery, u32 queryCount);

		void beginQuery(vk::QueryPool queryPool, u32 query, vk::QueryControlFlags flags);

		void endQuery(vk::QueryPool queryPool, u32 query);

		void writeTimestamp(vk::PipelineStageFlagBits pipelineStage, vk::QueryPool queryPool, u32 query);

		vk::Result reset(ResetMode resetMode);

	private:
		const bool isRenderSizeOptimal(vk::Extent2D & extent, vk::Rect2D & renderArea);

		void flushPipelineState(vk::PipelineBindPoint bindPoint);

		void flushDescriptorState(vk::PipelineBindPoint bindPoint);

	private:
		CommandPool & m_cmdPool; // NOLINT

		PipelineState m_pipelineState;

		ResourceBindingState m_resourceBindingState;



		vk::UniqueCommandBuffer m_handle{nullptr};
		std::string m_name;
		std::unique_ptr<Fence> m_waitFence;
	};
} // namespace gen