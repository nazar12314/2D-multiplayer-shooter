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
	deltaTime = glm::clamp(realTime - _lastTickTime, 0.0f, maxDeltaTime);
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
