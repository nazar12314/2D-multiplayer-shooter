#pragma once

#include <string>

#include <SDL_image.h>
#include <Color.h>

class Texture
{
public:
	SDL_Texture* texture;

	Texture(const std::string& path);
	Texture(const std::string& path, const Color& color);

private:
	void loadTexture(const std::string& path);
};
