// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vulkan/vulkan.hpp>

#include "gen/system/types.hpp"

#include <vector>

namespace gen
{
	struct VertexInputState
	{
		std::vector<vk::VertexInputBindingDescription> binding;
		std::vector<vk::VertexInputAttributeDescription> attribute;
	};

	struct InputAssemblyState
	{
		vk::PrimitiveTopology topology{vk::PrimitiveTopology::eTriangleList};
		vk::Bool32 primitiveRestartEnable{vk::False};
	};

	struct RasterizationState
	{
		vk::Bool32 depthClampEnable{vk::False};
		vk::Bool32 rasterizerDiscardEnable{vk::False};
		vk::PolygonMode polygonMode{vk::PolygonMode::eFill};
		vk::CullModeFlags cullMode{vk::CullModeFlagBits::eNone}; // TODO: Change this to cull back mode later
		vk::FrontFace frontFace{vk::FrontFace::eCounterClockwise};
		vk::Bool32 depthBiasEnable{vk::False};
	};

	struct ViewportState
	{
		u32 viewportCount{1};
		u32 scissorCount{1};
	};

	struct MultisampleState
	{
		vk::SampleCountFlagBits rasterizationSamples{vk::SampleCountFlagBits::e1};
		vk::Bool32 sampleShadingEnable{vk::False};
		float minSampleShading{1.0f}; // Change to 0 later
		vk::Bool32 alphaToCoverageEnable{vk::False};
		vk::Bool32 alphaToOneEnable{vk::False};
	};

	struct StencilOpState
	{
		vk::StencilOp failOp{vk::StencilOp::eReplace};
		vk::StencilOp passOp{vk::StencilOp::eReplace};
		vk::StencilOp depthFailOp{vk::StencilOp::eReplace};
		vk::CompareOp compareOp{vk::CompareOp::eNever};
	};

	struct DepthStencilState
	{
		vk::Bool32 depthTestEnable{vk::True};
		vk::Bool32 depthWriteEnable{vk::True};
		vk::CompareOp depthCompareOp{vk::CompareOp::eGreater}; // Note: We are using reversed depth buffer for better precision
		vk::Bool32 depthBoundsTestEnable{vk::False};
		vk::Bool32 stencilTestEnable{vk::False};
		StencilOpState front;
		StencilOpState back;
	};

	struct ColorBlendAttachmentState
	{
		vk::Bool32 blendEnable{vk::False};
		vk::BlendFactor srcColorBlendFactor{vk::BlendFactor::eOne};
		vk::BlendFactor dstColorBlendFactor{vk::BlendFactor::eZero};
		vk::BlendOp colorBlendOp{vk::BlendOp::eAdd};
		vk::BlendFactor srcAlphaBlendFactor{vk::BlendFactor::eOne};
		vk::BlendFactor dstAlphaBlendFactor{vk::BlendFactor::eZero};
		vk::BlendOp alphaBlendOp{vk::BlendOp::eAdd};
		vk::ColorComponentFlags colorWriteMask{
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};
	};

	struct ColorBlendState
	{
		vk::Bool32 logicOpEnable{vk::False};
		vk::LogicOp logicOp{vk::LogicOp::eClear};
		std::vector<ColorBlendAttachmentState> attachments;
	};

	class PipelineState
	{
	public:
		void reset();

		void setPipelineLayout();

		void setSpecializationConstants(u32 constantId, const std::vector<u8> & data);

		void setVertexInputState(const VertexInputState & vertexInputState);

		void setInputAssemblyState(const InputAssemblyState & inputAssemblyState);

		void setRasterizationState(const RasterizationState & rasterizationState);

		void setViewportState(const ViewportState & viewportState);

		void setMultisampleState(const MultisampleState & multisampleState);

		void setDepthStencilState(const DepthStencilState & depthStencilState);

		void setColorBlendState(const ColorBlendState & colorBlendState);

		void setSubpassIndex(u32 subpassIndex);

		const vk::PipelineLayout getPipelineLayout() const;
	};

} // namespace gen