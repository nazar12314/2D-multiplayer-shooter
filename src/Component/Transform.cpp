#include "Transform.h"

#include "DOTween.h"
#include "glm/gtx/rotate_vector.hpp"
#include "MyMath.h"
#include "glm/gtx/matrix_transform_2d.hpp"

Transform::Transform(GameObject* obj, const glm::vec2 pos, float rot) : Component(obj), _localPos(pos), _localRot(rot)
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

glm::vec2 Transform::pos()
{
	return getMatrixInv()[2];
}
float Transform::rot()
{
	return glm::degrees(glm::atan(getMatrixInv()[0][1], getMatrixInv()[1][1]));
}
glm::vec2 Transform::scale()
{
	return glm::vec2(length(getMatrixInv()[0]), length(getMatrixInv()[1]));
}
float Transform::z() const
{
	return _parent ? _localZ + _parent->z() : _localZ; // TODO: cache this
}

void Transform::setPos(glm::vec2 pos)
{
	this->_localPos = _parent ? _parent->globalToLocalPos(pos) : pos;
	recalculateLocalMatrix();
}
void Transform::setRot(float rot)
{
	this->_localRot = _parent ? rot - _parent->rot() : rot;
	recalculateLocalMatrix();
}
void Transform::setScale(glm::vec2 scale)
{
	this->_localScale = _parent ? scale / _parent->scale() : scale;
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
	setLocalPos(_localPos + v);
}
void Transform::rotate(float degrees)
{
	setLocalRot(_localRot + degrees);
}

void Transform::lookAt(const glm::vec2& target)
{
	setRot(glm::degrees(glm::atan(target.y - _localPos.y, target.x - _localPos.x)));
}

glm::vec2 Transform::up() { return glm::rotate(glm::vec2_up, glm::radians(rot())); }
glm::vec2 Transform::down() { return glm::rotate(glm::vec2_down, glm::radians(rot())); }
glm::vec2 Transform::left() { return glm::rotate(glm::vec2_left, glm::radians(rot())); }
glm::vec2 Transform::right() { return glm::rotate(glm::vec2_right, glm::radians(rot())); }

glm::vec2 Transform::localToWorldPos(const glm::vec2& pos)
{
	return getMatrixInv() * glm::vec3(pos, 1);
}
glm::vec2 Transform::globalToLocalPos(const glm::vec2& pos)
{
	return getMatrix() * glm::vec3(pos, 1);
}

Tween* Transform::doMove(const glm::vec2& endValue, float time)
{
	return DOVirtual::vec2To(pos(), endValue, time, [this](glm::vec2 pos) { setPos(pos); })->setTarget(this);
}
Tween* Transform::doRotate(float endValue, float time)
{
	return DOVirtual::floatTo(rot(), endValue, time, [this](float rot) { setRot(rot); })->setTarget(this);
}
Tween* Transform::doScale(const glm::vec2& endValue, float time)
{
	return DOVirtual::vec2To(scale(), endValue, time, [this](glm::vec2 scale) { setScale(scale); })->setTarget(this);
}

Tween* Transform::doLocalMove(const glm::vec2& endValue, float time)
{
	return DOVirtual::vec2To(_localPos, endValue, time, [this](glm::vec2 pos) { setLocalPos(pos); })->setTarget(this);
}
Tween* Transform::doLocalRotate(float endValue, float time)
{
	return DOVirtual::floatTo(_localRot, endValue, time, [this](float rot) { setLocalRot(rot); })->setTarget(this);
}
Tween* Transform::doLocalScale(const glm::vec2& endValue, float time)
{
	return DOVirtual::vec2To(_localScale, endValue, time, [this](glm::vec2 scale) { setLocalScale(scale); })->setTarget(this);
}

Tween* Transform::doMoveX(float endValue, float time)
{
	return DOVirtual::floatTo(pos().x, endValue, time, [this](float x) { setPos({x, pos().y}); })->setTarget(this);
}
Tween* Transform::doMoveY(float endValue, float time)
{
	return DOVirtual::floatTo(pos().y, endValue, time, [this](float y) { setPos({pos().x, y}); })->setTarget(this);
}
Tween* Transform::doLocalMoveX(float endValue, float time)
{
	return DOVirtual::floatTo(_localPos.x, endValue, time, [this](float x) { setLocalPos({x, _localPos.y}); })->setTarget(this);
}
Tween* Transform::doLocalMoveY(float endValue, float time)
{
	return DOVirtual::floatTo(_localPos.y, endValue, time, [this](float y) { setLocalPos({_localPos.x, y}); })->setTarget(this);
}
