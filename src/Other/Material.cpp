#include "Material.h"

#include "Assets.h"
#include "SDLHandler.h"

Material::Material(Texture* texture) : _texture(texture) {}
Material::Material(Texture* texture, const Color& color) : _texture(texture)
{
	setColor(color);
}
Material::~Material()
{
	if (_coloredSDLTexture != nullptr)
		SDL_DestroyTexture(_coloredSDLTexture);
}

SDL_Texture* Material::texture() const
{
	return _coloredSDLTexture != nullptr ? _coloredSDLTexture : _texture->texture();
}

void Material::setColor(const Color& color)
{
	if (_coloredSDLTexture == nullptr)
		_coloredSDLTexture = SDL_CreateTextureFromSurface(SDLHandler::renderer, _texture->surface());
	SDL_SetTextureColorMod(_coloredSDLTexture, color.r() * 255, color.g() * 255, color.b() * 255);
}
