#pragma once

#include <string>
#include "SDL_image.h"

class Texture
{
public:
	SDL_Texture* texture;

	Texture(const std::string& path)
	{
		SDL_Surface* surface = IMG_Load(path.data());
		texture = SDL_CreateTextureFromSurface(SDLHandler::renderer, surface);
		SDL_FreeSurface(surface);
	}
};
