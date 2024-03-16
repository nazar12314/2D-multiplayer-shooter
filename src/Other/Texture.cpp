#include "Texture.h"

#include "SDLHandler.h"

Texture::Texture(const std::string& path)
{
	loadTexture(path);
}

Texture::Texture(const std::string& path, const Color& color)
{
	loadTexture(path);
	SDL_SetTextureColorMod(texture, color.r() * 255, color.g() * 255, color.b() * 255);
}

void Texture::loadTexture(const std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.data());
	texture = SDL_CreateTextureFromSurface(SDLHandler::renderer, surface);
	SDL_FreeSurface(surface);
}
