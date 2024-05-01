#include "Assets.h"

#include <SDL_image.h>

#include "SDLHandler.h"

Asset::Asset(const std::string& path): _path(path) {}
const std::string& Asset::path() const { return _path; }

Sprite::Sprite(const std::string& path): Asset(path)
{
	_surface = IMG_Load(path.data());
	_defaultTex = SDL_CreateTextureFromSurface(SDLHandler::renderer, _surface);
}
Sprite::~Sprite()
{
	SDL_FreeSurface(_surface);
}

bool Sprite::satisfies() { return true; }

SDL_Surface* Sprite::surface() const { return _surface; }
SDL_Texture* Sprite::defaultTexture() const { return _defaultTex; }

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

TTF_Font* Font::font() const { return _font; }
