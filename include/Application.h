#pragma once

#include <SDL.h>

#include "glm/glm.hpp"

class Application
{
public:
	inline static SDL_Renderer* renderer;
	inline static SDL_Window* window;
	inline static SDL_Surface* surface;

	inline static glm::ivec2 windowSize;
	inline static bool windowFocused = true;
	inline static bool isFullscreen = false;

	inline static bool doQuit;

	static void init(const glm::ivec2& windowSize);
	static void initSDL(const glm::ivec2& windowSize);
	static void initScene();

	static void loop();

	static void quit();
	static void uninitSDL();
};
