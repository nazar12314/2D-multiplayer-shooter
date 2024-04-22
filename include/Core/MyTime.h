#pragma once

class Time
{
	inline static float _lastTickTime = 0;

	inline static float _time = 0;
	inline static float _deltaTime = 0;
	inline static float _unscaledDeltaTime = 0;

public:
	inline static float timeScale = 1;
	inline static float maxDeltaTime = 1 / 10.0f;
	inline static float fixedDeltaTime = 1 / 120.0f;

	static void init();

	static void tick();
	static void fixedTick();

	static float time() { return _time; }
	static float deltaTime() { return _deltaTime; }
	static float unscaledDeltaTime() { return _unscaledDeltaTime; }

	static float getRealTime();

	friend class FPSCounter;
};
