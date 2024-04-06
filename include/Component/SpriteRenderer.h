#pragma once

#include "BaseRenderer.h"
#include "Color.h"
#include "glm/vec2.hpp"

class Sprite;

class SpriteRenderer : public BaseRenderer
{
	SpriteRenderer(GameObject* obj, const Sprite* texture, glm::vec2 size = {1, 1}, const Color& color = Color::white, int order = 0);

	friend class GameObject;
};
