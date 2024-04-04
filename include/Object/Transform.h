#pragma once

#include "glm/vec2.hpp"
#include "Action.h"

class Transform
{
	glm::vec2 _pos;
	float _rot;
	glm::vec2 _scale = {1, 1};

	float _z = 0;

protected:
	Transform(glm::vec2 pos = {0, 0}, float rot = 0);

public:
	glm::vec2 pos() const { return _pos; }
	float rot() const { return _rot; }
	glm::vec2 scale() const { return _scale; }
	float z() const { return _z; }

	Action<float> onRotChange;
	void setPos(glm::vec2 pos);
	void setRot(float rot);
	void setScale(glm::vec2 scale);
	void setZ(float z);

	void translate(const glm::vec2& v);
	void rotate(float degrees);

	void lookAt(const glm::vec2& target);

	glm::vec2 up() const;
	glm::vec2 down() const;
	glm::vec2 left() const;
	glm::vec2 right() const;

	glm::vec2 localToGlobalPos(const glm::vec2& pos) const;
	glm::vec2 globalToLocalPos(const glm::vec2& pos) const;
};
