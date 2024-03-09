#pragma once

#include "Object.h"

class Camera : public Object
{
	inline static std::vector<Object*> cameras {};

public:
	int size;
	int priority = 0;

	Camera(const glm::vec2& pos, float rot, int size);

	void render() const;

	static void renderMain();
};
