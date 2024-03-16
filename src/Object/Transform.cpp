#include "Transform.h"
#include "glm/trigonometric.hpp"
#include "glm/gtx/rotate_vector.hpp"

Transform::Transform(const glm::vec2 pos, float rot) : pos(pos), rot(rot) {}

void Transform::setPos(glm::vec2 pos) { this->pos = pos; }
void Transform::setRot(float rot) {
    this->rot = rot;
    onRotChange(rot);
}
void Transform::translate(const glm::vec2& v)
{
	setPos(pos + v);
}
void Transform::rotate(float degrees)
{
	setRot(rot + degrees);
}

glm::vec2 Transform::up() const { return glm::rotate(glm::vec2(0, 1), rot); }
glm::vec2 Transform::down() const { return glm::rotate(glm::vec2(0, -1), rot); }
glm::vec2 Transform::left() const { return glm::rotate(glm::vec2(-1, 0), rot); }
glm::vec2 Transform::right() const { return glm::rotate(glm::vec2(1, 0), rot); }

glm::vec2 Transform::localToGlobalPos(const glm::vec2& localPos) const
{
	return glm::rotate(localPos, rot) + pos;
}
glm::vec2 Transform::globalToLocalPos(const glm::vec2& globalPos) const
{
	return glm::rotate(globalPos, -rot) - pos;
}
