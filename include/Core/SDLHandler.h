#pragma once
#include <SDL_render.h>

#include "glm/vec2.hpp"

class SDLHandler
{
public:
	inline static SDL_Renderer* renderer;
	inline static SDL_Window* window;
	inline static SDL_Surface* surface;

	inline static glm::ivec2 windowSize;

	static void init(const glm::ivec2& windowSize);
	static void handleEvents();
	static void uninit();
};
