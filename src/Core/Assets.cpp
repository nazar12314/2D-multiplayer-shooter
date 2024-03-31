#include "Assets.h"

#include <SDL_image.h>

#include "SDLHandler.h"

void Assets::init() {}

TextureBase::TextureBase(const std::string& path): Asset(path)
{
	_surface = IMG_Load(path.data());
}
TextureBase::~TextureBase()
{
	SDL_FreeSurface(_surface);
}

Texture::Texture(TextureBase* base) : _base(base)
{
	_texture = SDL_CreateTextureFromSurface(SDLHandler::renderer, base->surface());
}
Texture::Texture(const std::string& path)
{
	_base = Assets::load<TextureBase>(path);
	_texture = SDL_CreateTextureFromSurface(SDLHandler::renderer, _base->surface());
}
Texture::Texture(const Texture& other) : _base(other._base)
{
	_texture = SDL_CreateTextureFromSurface(SDLHandler::renderer, _base->surface());
}

void Texture::setColor(const Color& color) const
{
	SDL_SetTextureColorMod(_texture, color.r() * 255, color.g() * 255, color.b() * 255);
}
