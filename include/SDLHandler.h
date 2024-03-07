#pragma once

#include <SDL.h>

class SDLHandler
{
public:
	inline static SDL_Renderer* renderer;
	inline static SDL_Window* window;
	inline static SDL_Surface* surface;
	inline static SDL_Event event;

	inline static bool doQuit;

	inline static bool windowFocused = true;
	inline static bool isFullscreen = false;

	static void init(int width, int height);
	static void initSDL(int width, int height);
	static void initScene();

	static void loop();
	static bool update();

	static void quit();
	static void uninitSDL();
};
