#pragma once

class Time
{
	inline static float lastTickTime = 0;

public:
	inline static float time = 0;
	inline static float deltaTime = 0;
	inline static float fixedDeltaTime = 1 / 60.0f;
	inline static int fixedUpdateCount = 0;

	static void init();

	static void tick();
	static void fixedTick();

	static float getRealTime();
};
