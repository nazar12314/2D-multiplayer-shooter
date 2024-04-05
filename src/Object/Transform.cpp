#include "Transform.h"

#include "glm/gtx/rotate_vector.hpp"
#include "MyMath.h"
#include "glm/gtx/matrix_transform_2d.hpp"

Transform::Transform(Object* obj, const glm::vec2 pos, float rot) : Component(obj), _localPos(pos), _localRot(rot)
{
	recalculateLocalMatrix();
}

void Transform::recalculateLocalMatrix()
{
	_localMatrixInv = glm::translate(glm::mat3_identity, _localPos);
	_localMatrixInv = glm::rotate(_localMatrixInv, glm::radians(_localRot));
	_localMatrixInv = glm::scale(_localMatrixInv, _localScale);
	_localMatrix = inverse(_localMatrixInv);

	markDirtyRec();
}
void Transform::recalculateGlobalMatrix()
{
	_matrixCached = _parent ? _localMatrix * _parent->getMatrix() : _localMatrix;
	_matrixInvCached = inverse(_matrixCached);
}

void Transform::markDirtyRec()
{
	isDirty = true;
	hasChanged = true;
	for (Transform* child : _children)
		child->markDirtyRec();
}

void Transform::setLocalPos(glm::vec2 pos)
{
	this->_localPos = pos;
	recalculateLocalMatrix();
}
void Transform::setLocalRot(float rot)
{
	this->_localRot = rot;
	recalculateLocalMatrix();
}
void Transform::setLocalScale(glm::vec2 scale)
{
	this->_localScale = scale;
	recalculateLocalMatrix();
}
void Transform::setLocalZ(float z)
{
	this->_localZ = z;
	markDirtyRec();
}
void Transform::setParent(Transform* parent)
{
	if (_parent) std::erase(_parent->_children, this);
	_parent = parent;
	if (_parent) _parent->_children.push_back(this);

	markDirtyRec();
}

glm::vec2 Transform::getPos()
{
	return getMatrixInv()[2];
}
float Transform::getRot()
{
	return glm::degrees(glm::atan(getMatrixInv()[0][1], getMatrixInv()[1][1]));
}
glm::vec2 Transform::getScale()
{
	return glm::vec2(length(getMatrixInv()[0]), length(getMatrixInv()[1]));
}
float Transform::getZ() const
{
	return _parent ? _localZ + _parent->getZ() : _localZ; // TODO: cache this
}

void Transform::setPos(glm::vec2 pos)
{
	this->_localPos = _parent ? _parent->globalToLocalPos(pos) : pos;
	recalculateLocalMatrix();
}
void Transform::setRot(float rot)
{
	this->_localRot = _parent ? rot - _parent->getRot() : rot;
	recalculateLocalMatrix();
}
void Transform::setScale(glm::vec2 scale)
{
	this->_localScale = _parent ? scale / _parent->getScale() : scale;
	recalculateLocalMatrix();
}
void Transform::setZ(float z) { this->_localZ = z; }


glm::mat3 Transform::getMatrix()
{
	if (isDirty)
	{
		recalculateGlobalMatrix();
		isDirty = false;
	}
	return _matrixCached;
}
glm::mat3 Transform::getMatrixInv()
{
	if (isDirty)
	{
		recalculateGlobalMatrix();
		isDirty = false;
	}
	return _matrixInvCached;
}

void Transform::translate(const glm::vec2& v)
{
	setPos(_localPos + v);
}
void Transform::rotate(float degrees)
{
	setRot(_localRot + degrees);
}

void Transform::lookAt(const glm::vec2& target)
{
	setRot(glm::degrees(glm::atan(target.y - _localPos.y, target.x - _localPos.x)));
}

glm::vec2 Transform::up() { return glm::rotate(glm::vec2_up, glm::radians(getRot())); }
glm::vec2 Transform::down() { return glm::rotate(glm::vec2_down, glm::radians(getRot())); }
glm::vec2 Transform::left() { return glm::rotate(glm::vec2_left, glm::radians(getRot())); }
glm::vec2 Transform::right() { return glm::rotate(glm::vec2_right, glm::radians(getRot())); }

glm::vec2 Transform::localToGlobalPos(const glm::vec2& pos)
{
	return getMatrixInv() * glm::vec3(pos, 1);
}
glm::vec2 Transform::globalToLocalPos(const glm::vec2& pos)
{
	return getMatrix() * glm::vec3(pos, 1);
}
