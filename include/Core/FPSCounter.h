#pragma once

#include <iostream>

#include "MyTime.h"

class FPSCounter
{
	inline static float lastTime = 0;
	inline static int frames = 0;

	constexpr static float FPS_UPDATE_DELAY = 0.3f;
	inline static float lastFPSPrintTime = 0;

public:
	inline static bool showFPS = false;
	inline static float fps = 0;

	static void tick()
	{
		frames++;
		if (Time::_time - lastTime >= FPS_UPDATE_DELAY)
		{
			fps = frames / (Time::_time - lastTime);
			lastTime = Time::_time;
			frames = 0;
		}

		if (showFPS && Time::_time - lastFPSPrintTime >= FPS_UPDATE_DELAY)
		{
			lastFPSPrintTime = Time::_time;
			std::cout << "FPS: " << fps << std::endl;
		}
	}
};
