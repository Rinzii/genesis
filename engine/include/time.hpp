#pragma once
#include <chrono>

namespace gen {
namespace Time {
using Clock = std::chrono::high_resolution_clock;

void UpdateDeltaTime();

void SetTimeScale(double timeScale);
double GetDeltaTime();
double GetTimeScale();

std::string GetCurrentTime();
} // namespace Time

namespace FPS {
void SetFPSUpdateDelay(double updateDelay);
void UpdateFPS();
unsigned GetFPS();
} // namespace FPS
} // namespace gen
