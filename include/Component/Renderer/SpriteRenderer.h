#pragma once

#include "BaseRenderer.h"
#include "Color.h"

class Sprite;

class SpriteRenderer : public RectRenderer
{
	SpriteRenderer(GameObject* obj, const Sprite* sprite, glm::vec2 size = {1, 1}, const Color& color = Color::WHITE, int order = 0);

public:
	void setSprite(const Sprite* sprite);

	friend class GameObject;
};
