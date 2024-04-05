#pragma once

#include <SDL_render.h>
#include <string>

#include "Color.h"
#include "glm/vec2.hpp"

class Font;
class Color;
class Sprite;

class Texture
{
	SDL_Surface* _surface;
	SDL_Texture* _texture;
	glm::vec2 _size;

public:
	Texture(const Sprite* sprite);
	Texture(const Font* font, const std::string& text, const Color& color);

	SDL_Surface* surface() const;
	SDL_Texture* texture() const;
	glm::vec2 size() const;

	float getRatio() const;

	friend class Material;
};

class Material
{
	Texture _texture;
	SDL_Texture* _clonedSDLTexture = nullptr;

public:
	Material(const Texture& texture, const Color& color = Color::white);
	~Material();

	const Texture& texture() const;
	SDL_Texture* sdlTexture() const;

	void setTexture(const Texture& texture);
	void setColor(const Color& color);
};

