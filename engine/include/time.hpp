#pragma once
#include <chrono>

namespace gen
{
	namespace Time
	{
		using Clock = std::chrono::high_resolution_clock;

		void UpdateDeltaTime();

		void SetTimeScale(double timeScale);
		double GetDeltaTime();
		double GetTimeScale();
	}

	namespace FPS
	{
		void SetFPSUpdateDelay(double updateDelay);
		void UpdateFPS();
		unsigned GetFPS();
	}
}
