#include "Transform.h"

Transform::Transform(const glm::vec2 pos, float rot) : pos(pos), rot(rot) {}

void Transform::setPos(glm::vec2 pos) { this->pos = pos; }
void Transform::setRot(float rot) { this->rot = rot; }
void Transform::translate(const glm::vec2& v)
{
	setPos(pos + v);
}
void Transform::rotate(float degrees)
{
	setRot(rot + degrees);
}

glm::vec2 Transform::up() const { return rot * glm::vec2(0, 1); }
glm::vec2 Transform::down() const { return rot * glm::vec2(0, -1); }
glm::vec2 Transform::left() const { return rot * glm::vec2(-1, 0); }
glm::vec2 Transform::right() const { return rot * glm::vec2(1, 0); }

glm::vec2 Transform::localToGlobalPos(const glm::vec2& localPos) const
{
	return rot * localPos + pos;
}
glm::vec2 Transform::globalToLocalPos(const glm::vec2& globalPos) const
{
	return -rot * globalPos - pos;
}
