#pragma once

#include "Color.h"
#include "Component.h"
#include "glm/vec2.hpp"

class Texture;

class SpriteRenderer : public Component
{
	Texture* _texture;
	glm::vec2 _size;
	int _order = 0;

	bool clonedTexture = false;

	SpriteRenderer(Object* obj, Texture* texture, glm::vec2 size = {1, 1}, int order = 0, const Color& color = Color::white);

public:
	Texture* sprite() const;
	glm::vec2 size() const;
	int order() const;

	void setNativeSize();
	void setOrder(int order);
	void setColor(const Color& color);

	void onDestroy() override;

	friend class Object;
	friend class Renderer;
};
