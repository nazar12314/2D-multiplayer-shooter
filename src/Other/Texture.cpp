#include "Texture.h"

#include "SDLHandler.h"

Texture::Texture(const std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.data());
	texture = SDL_CreateTextureFromSurface(SDLHandler::renderer, surface);
	SDL_FreeSurface(surface);
}
