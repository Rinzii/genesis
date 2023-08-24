// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
#include <chrono>

namespace gen
{
	namespace Time
	{
		using Clock = std::chrono::steady_clock;

		void UpdateDeltaTime();

		void SetTimeScale(double timeScale);
		float GetDeltaTime();
		float GetTimeScale();

		std::string GetCurrentTime();
	} // namespace Time

	namespace FPS
	{
		void SetFPSUpdateDelay(double updateDelay);
		void UpdateFPS();
		unsigned GetFPS();
	} // namespace FPS
} // namespace gen
