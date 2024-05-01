#include "Texture.h"

#include <iostream>

#include "Assets.h"
#include "SDLHandler.h"
#include "glm/common.hpp"

void Texture::setColor(const Color& color)
{
	if (_usingDefaultTexture)
	{
		_texture = SDL_CreateTextureFromSurface(SDLHandler::renderer, _surface);
		_usingDefaultTexture = false;
	}
	SDL_SetTextureColorMod(_texture, glm::clamp(color.r(), 0.0f, 1.0f) * 255, glm::clamp(color.g(), 0.0f, 1.0f) * 255, glm::clamp(color.b(), 0.0f, 1.0f) * 255);
	SDL_SetTextureAlphaMod(_texture, glm::clamp(color.a(), 0.0f, 1.0f) * 255);
}

Texture::Texture(const Sprite* sprite, const Color& color) : _surface(sprite->surface()), _texture(sprite->defaultTexture()), _usingDefaultTexture(true)
{
	_size = {_surface->w, _surface->h};

	if (color != Color::WHITE)
		setColor(color);
}
Texture::Texture(const Font* font, const std::string& text, const Color& color) : _usingDefaultTexture(false)
{
	_surface = TTF_RenderText_Blended(font->font(), text.data(), color.toSDLColor());
	if (_surface == nullptr) return;

	_texture = SDL_CreateTextureFromSurface(SDLHandler::renderer, _surface);
	_size = {_surface->w, _surface->h};
}
Texture::~Texture()
{
	if (!_usingDefaultTexture)
		SDL_DestroyTexture(_texture);
}

SDL_Surface* Texture::surface() const { return _surface; }
SDL_Texture* Texture::texture() const
{
	return _texture;
}
glm::vec2 Texture::size() const { return _size; }

float Texture::getRatio() const { return _size.x / _size.y; }
