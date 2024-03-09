#pragma once

class Time
{
public:
	inline static float time = 0;
	inline static float deltaTime = 0;

	static void tick()
	{
		auto currTime = (float)SDL_GetTicks() / 1000.0f;
		deltaTime = currTime - time;
		time = currTime;
	}
};
