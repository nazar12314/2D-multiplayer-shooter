#pragma once

#include "Component.h"
#include "glm/vec2.hpp"

class Texture;

class Sprite : public Component
{
	Texture* _texture;
	glm::vec2 _size;
	int _order = 0;

	Sprite(Object* obj, Texture* texture, glm::vec2 size = {1, 1}, int order = 0);

public:
	Texture* texture() const;
	glm::vec2 size() const;
	int order() const;

	void setNativeSize();
	void setOrder(int order);

	friend class Object;
	friend class Renderer;
};
