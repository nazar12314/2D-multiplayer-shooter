#pragma once
#include <iostream>

class FPSCounter
{
	inline static float lastTime = 0;
	inline static int frames = 0;

	constexpr static float FPS_PRINT_DELAY = 1;
	inline static float lastFPSPrintTime = 0;

public:
	inline static bool showFPS = true;
	inline static float fps = 0;

	static void tick()
	{
		frames++;
		if (Time::time - lastTime >= 1)
		{
			fps = frames / (Time::time - lastTime);
			lastTime = Time::time;
			frames = 0;
		}

		if (showFPS && Time::time - lastFPSPrintTime >= FPS_PRINT_DELAY)
		{
			lastFPSPrintTime = Time::time;
			std::cout << "FPS: " << fps << std::endl;
		}
	}
};
