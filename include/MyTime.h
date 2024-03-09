#pragma once

class Time
{
public:
	inline static float time = 0;
	inline static float deltaTime = 0;

	static void tick()
	{
		auto new_time = (float)SDL_GetTicks() / 1000.0f;
		deltaTime = new_time - time;
		time = new_time;
	}
};
