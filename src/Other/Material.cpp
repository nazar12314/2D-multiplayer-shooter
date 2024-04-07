#include "Material.h"

#include "Assets.h"
#include "SDLHandler.h"

Material::Material(const Texture& texture, const Color& color) : _texture(texture)
{
	if (color != Color::white)
		setColor(color);
}
Material::~Material()
{
	if (_clonedSDLTexture != nullptr)
		SDL_DestroyTexture(_clonedSDLTexture);
}

const Texture& Material::texture() const
{
	return _texture;
}
SDL_Texture* Material::sdlTexture() const
{
	return _clonedSDLTexture != nullptr ? _clonedSDLTexture : _texture.texture();
}

void Material::setTexture(const Texture& texture)
{
	this->_texture = texture;

	if (_clonedSDLTexture != nullptr)
	{
		SDL_DestroyTexture(_clonedSDLTexture);
		_clonedSDLTexture = nullptr;
	}
}

void Material::setColor(const Color& color)
{
	if (_clonedSDLTexture == nullptr)
		_clonedSDLTexture = SDL_CreateTextureFromSurface(SDLHandler::renderer, _texture.surface());
	SDL_SetTextureColorMod(_clonedSDLTexture, color.r() * 255, color.g() * 255, color.b() * 255);
}

Texture::Texture(const Sprite* sprite) : _surface(sprite->surface()), _texture(sprite->defaultTexture()), _usingDefaultTexture(true)
{
	_size = {_surface->w, _surface->h};
}
Texture::Texture(const Font* font, const std::string& text, const Color& color): _usingDefaultTexture(false)
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
SDL_Texture* Texture::texture() const { return _texture; }
glm::vec2 Texture::size() const { return _size; }

float Texture::getRatio() const { return _size.x / _size.y; }
