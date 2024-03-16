#pragma once

#include "glm/vec2.hpp"

class Transform
{
protected:
	Transform(glm::vec2 pos = {0, 0}, float rot = 0);
	~Transform() = default;

public:
	glm::vec2 pos;
	float rot;
	glm::vec2 scale = {1, 1};


	glm::vec2 getPos() const { return pos; }
	float getRot() const { return rot; }
	virtual void setPos(glm::vec2 pos);
	virtual void setRot(float rot);

	void translate(const glm::vec2& v);
	void rotate(float degrees);

	glm::vec2 up() const;
	glm::vec2 down() const;
	glm::vec2 left() const;
	glm::vec2 right() const;

	glm::vec2 localToGlobalPos(const glm::vec2& localPos) const;
	glm::vec2 globalToLocalPos(const glm::vec2& globalPos) const;
};
