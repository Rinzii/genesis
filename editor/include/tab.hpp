#pragma once
#include "imgui.h"
#include "base/config/compilerTraits.hpp"
#include "mim/vec3.hpp"

namespace gen
{
	namespace editor
	{
		class Tab
		{
		public:

			Tab();
			virtual ~Tab();

			Tab(const Tab& other) = delete;
			Tab(Tab&& other) noexcept = delete;
			Tab& operator=(const Tab& other) = delete;
			Tab& operator=(Tab&& other) noexcept = delete;

			virtual void Update() = 0;

			GEN_NODISCARD float GetWidth() const;
			GEN_NODISCARD float GetHeight() const;

		protected:

			float m_width;
			float m_height;
		};

		class WindowTab : public Tab
		{
			
		};

		class ModalTab : public Tab
		{
			
		};

		class TestTab final : public ModalTab
		{
			void Update() override;
		};
}
}
