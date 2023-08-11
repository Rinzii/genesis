// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "time.hpp"

namespace gen
{
	namespace Time
	{
		namespace
		{
			Clock::time_point g_start{Clock::now()};
			double g_deltaTime;
			double g_timeScale;
		} // namespace

		void UpdateDeltaTime()
		{
			auto const end = Clock::now();
			g_deltaTime	   = std::chrono::duration_cast<std::chrono::duration<double>>(end - g_start).count() * g_timeScale;
			g_start		   = end;
		}

		void SetTimeScale(double timeScale)
		{
			g_timeScale = timeScale;
		}
		double GetDeltaTime()
		{
			return g_deltaTime;
		}
		double GetTimeScale()
		{
			return g_timeScale;
		}

		std::string GetCurrentTime()
		{
			auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());

			return std::format("{:%Y-%m-%d %X}", time);
		}
	} // namespace Time

	namespace FPS
	{
		namespace
		{
			double g_updateDelay = 0.5;
			double g_updateTimer;
			unsigned g_fps;
		} // namespace

		void SetFPSUpdateDelay(double updateDelay)
		{
			g_updateDelay = updateDelay;
		}

		void UpdateFPS()
		{

			g_updateTimer += Time::GetDeltaTime();

			if (g_updateTimer >= g_updateDelay)
			{
				g_updateTimer = 0;
				g_fps		  = static_cast<unsigned int>(1 / (Time::GetDeltaTime() / Time::GetTimeScale()));
			}
		}

		unsigned GetFPS()
		{
			return g_fps;
		}
	} // namespace FPS
} // namespace gen
