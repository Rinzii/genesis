#pragma once

namespace gen
{
	class RenderFrame
	{
	public:
		RenderFrame()  = default;
		~RenderFrame() = default;

		RenderFrame(const RenderFrame &)			 = delete;
		RenderFrame(RenderFrame &&)					 = delete;
		RenderFrame & operator=(const RenderFrame &) = delete;
		RenderFrame & operator=(RenderFrame &&)		 = delete;

	private:
	};
} // namespace gen
