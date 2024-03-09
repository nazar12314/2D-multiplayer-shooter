#pragma once

#include "Object.h"

class Graphical : public Object
{
	Texture* texture;
	glm::ivec2 size;

public:
	inline static std::vector<Graphical*> graphicalObjects {};

	Graphical(Texture* texture, glm::ivec2 size = {1, 1}, const glm::vec2& pos = {0, 0}, float rot = 0);
	~Graphical() override;

	void draw(const glm::vec2& cameraPos, int cameraSize) const;

	void setNativeSize();
};
