#pragma once

#include <string>

#include "Application.h"
#include <SDL_image.h>

class Texture
{
public:
	SDL_Texture* texture;

	Texture(const std::string& path)
	{
		SDL_Surface* surface = IMG_Load(path.data());
		texture = SDL_CreateTextureFromSurface(Application::renderer, surface);
		SDL_FreeSurface(surface);
	}
};
