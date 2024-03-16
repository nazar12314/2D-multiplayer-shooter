#pragma once

#include "Component.h"
#include "glm/vec2.hpp"

class Texture;

class Sprite : public Component
{
	Texture* _texture;
	glm::vec2 _size;

	Sprite(Object* obj, Texture* texture, glm::vec2 size = {1, 1});

public:
	Texture* texture() const;
	glm::vec2 size() const;

	void setNativeSize();

	friend class Object;
};
