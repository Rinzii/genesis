// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/time.hpp"

namespace gen
{
	namespace Time
	{
		namespace
		{
			Clock::time_point g_start{Clock::now()};
			float g_deltaTime;
			float g_timeScale;
		} // namespace

		void UpdateDeltaTime()
		{
			auto const end = Clock::now();
			g_deltaTime	   = std::chrono::duration_cast<std::chrono::duration<float>>(end - g_start).count() * g_timeScale;
			g_start		   = end;
		}

		void SetTimeScale(float timeScale)
		{
			g_timeScale = timeScale;
		}

		float GetDeltaTime()
		{
			return g_deltaTime;
		}

		float GetTimeScale()
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
			float g_updateDelay = 0.5;
			float g_updateTimer;
			unsigned g_fps;
		} // namespace

		void SetFPSUpdateDelay(float updateDelay)
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
