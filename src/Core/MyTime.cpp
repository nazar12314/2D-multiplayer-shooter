#include "MyTime.h"

#include <SDL_timer.h>

#include "glm/common.hpp"

void Time::init()
{
	_time = getRealTime();
}

void Time::tick()
{
	auto realTime = getRealTime();
	_unscaledDeltaTime = glm::clamp(realTime - _lastTickTime, 0.0f, maxDeltaTime);
	_deltaTime = _unscaledDeltaTime * timeScale;
	_time = _lastTickTime = realTime;
}

void Time::fixedTick()
{
	_time += fixedDeltaTime;
}

float Time::getRealTime()
{
	return (float)SDL_GetTicks() / 1000.0f;
}
