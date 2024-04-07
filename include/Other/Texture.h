#pragma once

#include <SDL_render.h>
#include <string>

#include "Color.h"
#include "glm/vec2.hpp"

class Font;
class Sprite;

class Texture
{
	SDL_Surface* _surface;
	SDL_Texture* _texture;
	glm::vec2 _size;

	bool _usingDefaultTexture;

public:
	Texture(const Sprite* sprite, const Color& color = Color::white);
	Texture(const Font* font, const std::string& text, const Color& color);
	~Texture();

	SDL_Surface* surface() const;
	SDL_Texture* texture() const;
	glm::vec2 size() const;

	float getRatio() const;

	void setColor(const Color& color);
};
