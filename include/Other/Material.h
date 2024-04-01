#pragma once

#include <SDL_render.h>

class Color;
class Texture;

class Material
{
	Texture* _texture;
	SDL_Texture* _coloredSDLTexture = nullptr;

public:
	Material(Texture* texture);
	~Material();

	SDL_Texture* texture() const;

	void setColor(const Color& color);
};
