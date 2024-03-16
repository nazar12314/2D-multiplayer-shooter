#pragma once

#include "glm/glm.hpp"

class Application
{
public:
	inline static bool windowFocused = true;
	inline static bool isFullscreen = false;

	inline static bool doQuit;

	static void start(const glm::ivec2& windowSize);
	static void loop();
	static void quit();
};
