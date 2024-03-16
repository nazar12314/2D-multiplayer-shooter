#pragma once

#include "Component.h"
#include "Object.h"

class Graphical : public Component
{
	Texture* texture;
	glm::ivec2 size;

	Graphical(Object* obj, Texture* texture, glm::ivec2 size = {1, 1});

public:
	void draw(const glm::vec2& cameraPos, int cameraSize) const;

	void setNativeSize();

	friend class Object;
};