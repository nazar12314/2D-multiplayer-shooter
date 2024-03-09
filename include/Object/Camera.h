#pragma once

#include "Object.h"

class Camera : public Object
{
	inline static std::vector<Object*> cameras {};

public:
	int size;
	int priority = 0;

	Camera(int size, const glm::vec2& pos = {0, 0}, float rot = 0);
	~Camera() override;
	void render() const;

	static void renderMain();
};
