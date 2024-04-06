#pragma once

class Time
{
	inline static float _lastTickTime = 0;

public:
	inline static float maxDeltaTime = 1 / 10.0f;
	inline static float fixedDeltaTime = 1 / 120.0f;

	inline static float timeScale = 1;
	inline static float time = 0;
	inline static float deltaTime = 0;
	inline static float unscaledDeltaTime = 0;

	static void init();

	static void tick();
	static void fixedTick();

	static float getRealTime();
};
