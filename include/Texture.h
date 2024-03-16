#pragma once

#include <string>

#include <SDL_image.h>

class Texture
{
public:
	SDL_Texture* texture;

	Texture(const std::string& path);
};
