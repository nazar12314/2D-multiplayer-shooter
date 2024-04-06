#include "MyTime.h"

#include <SDL_timer.h>

#include "glm/common.hpp"

void Time::init()
{
	time = getRealTime();
}

void Time::tick()
{
	auto realTime = getRealTime();
	unscaledDeltaTime = glm::clamp(realTime - _lastTickTime, 0.0f, maxDeltaTime);
	deltaTime = unscaledDeltaTime * timeScale;
	time = _lastTickTime = realTime;
}

void Time::fixedTick()
{
	time += fixedDeltaTime;
}

float Time::getRealTime()
{
	return (float)SDL_GetTicks() / 1000.0f;
}
