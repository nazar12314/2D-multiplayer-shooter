#include "MyTime.h"

#include <SDL_timer.h>

void Time::init()
{
	time = getRealTime();
}

void Time::tick()
{
	auto realTime = getRealTime();
	deltaTime = realTime - lastTickTime;
	time = lastTickTime = realTime;
}

void Time::fixedTick()
{
	time += fixedDeltaTime;
}

float Time::getRealTime()
{
	return (float)SDL_GetTicks() / 1000.0f;
}
