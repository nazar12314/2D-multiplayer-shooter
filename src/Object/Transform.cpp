#include "Transform.h"
#include "glm/trigonometric.hpp"
#include "glm/gtx/rotate_vector.hpp"

Transform::Transform(const glm::vec2 pos, float rot) : _pos(pos), _rot(rot) {}

void Transform::setPos(glm::vec2 pos)
{
	this->_pos = pos;
	transformChanged = true;
}
void Transform::setRot(float rot)
{
	this->_rot = rot;
	transformChanged = true;
}
void Transform::setScale(glm::vec2 scale)
{
	this->_scale = scale;
	transformChanged = true;
}
void Transform::translate(const glm::vec2& v)
{
	setPos(_pos + v);
}
void Transform::rotate(float degrees)
{
	setRot(_rot + degrees);
}

void Transform::lookAt(const glm::vec2& target)
{
	setRot(glm::degrees(glm::atan(target.y - _pos.y, target.x - _pos.x)));
}

glm::vec2 Transform::up() const { return glm::rotate(glm::vec2(0, 1), glm::radians(-_rot)); }
glm::vec2 Transform::down() const { return glm::rotate(glm::vec2(0, -1), glm::radians(-_rot)); }
glm::vec2 Transform::left() const { return glm::rotate(glm::vec2(-1, 0), glm::radians(-_rot)); }
glm::vec2 Transform::right() const { return glm::rotate(glm::vec2(1, 0), glm::radians(-_rot)); }

glm::vec2 Transform::localToGlobalPos(const glm::vec2& localPos) const
{
	return glm::rotate(localPos, _rot) + _pos;
}
glm::vec2 Transform::globalToLocalPos(const glm::vec2& globalPos) const
{
	return glm::rotate(globalPos, -_rot) - _pos;
}
