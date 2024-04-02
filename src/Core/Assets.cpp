#include "Assets.h"

#include <SDL_image.h>

#include "SDLHandler.h"

Texture::Texture(const std::string& path): Asset(path)
{
	_surface = IMG_Load(path.data());
	_texture = SDL_CreateTextureFromSurface(SDLHandler::renderer, _surface);
}
Texture::~Texture()
{
	SDL_FreeSurface(_surface);
}
