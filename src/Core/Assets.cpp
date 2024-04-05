#include "Assets.h"

#include <SDL_image.h>

#include "SDLHandler.h"

Sprite::Sprite(const std::string& path): Asset(path)
{
	_surface = IMG_Load(path.data());
	_defaultTex = SDL_CreateTextureFromSurface(SDLHandler::renderer, _surface);
}
Sprite::~Sprite()
{
	SDL_FreeSurface(_surface);
}

Font::Font(const std::string& path, int size): Asset(path)
{
	this->_size = size;

	_font = TTF_OpenFont(path.data(), size);
}
Font::~Font()
{
	TTF_CloseFont(_font);
}

bool Font::satisfies(int size) const
{
	return this->_size == size;
}
