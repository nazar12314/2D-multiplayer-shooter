#pragma once

#include "glm/vec2.hpp"
#include "Action.h"

class Transform
{
	glm::vec2 _pos;
	float _rot;
	glm::vec2 _scale = {1, 1};

protected:
	Transform(glm::vec2 pos = {0, 0}, float rot = 0);

public:
	bool transformChanged = false;

	glm::vec2 pos() const { return _pos; }
	float rot() const { return _rot; }
	glm::vec2 scale() const { return _scale; }

    Action<float> onRotChange;
	void setPos(glm::vec2 pos);
	void setRot(float rot);
	void setScale(glm::vec2 scale);

	void translate(const glm::vec2& v);
	void rotate(float degrees);

	glm::vec2 up() const;
	glm::vec2 down() const;
	glm::vec2 left() const;
	glm::vec2 right() const;

	glm::vec2 localToGlobalPos(const glm::vec2& localPos) const;
	glm::vec2 globalToLocalPos(const glm::vec2& globalPos) const;
};
