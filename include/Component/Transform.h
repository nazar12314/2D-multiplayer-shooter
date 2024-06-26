#pragma once

#include "Component.h"
#include "glm/vec2.hpp"
#include "glm/mat3x3.hpp"

class Tween;

class Transform : public Component
{
	glm::vec2 _localPos;
	float _localRot;
	glm::vec2 _localScale = {1, 1};
	float _localZ = 0;

	glm::mat3 _localMatrix = glm::mat3(1);
	glm::mat3 _localMatrixInv = glm::mat3(1);

	bool isDirty = true;
	bool hasChanged = true; // Used by Rigidbody

	// Caching global matrix
	glm::mat3 _matrixCached = glm::mat3(1);
	glm::mat3 _matrixInvCached = glm::mat3(1);

	Transform* _parent = nullptr;
	std::vector<Transform*> _children;

	Transform(GameObject* obj, glm::vec2 pos = {0, 0}, float rot = 0);

	void recalculateLocalMatrix();
	void recalculateGlobalMatrix();

	void markDirtyRec();

public:
	glm::vec2 localPos() const { return _localPos; }
	float localRot() const { return _localRot; }
	glm::vec2 localScale() const { return _localScale; }
	float localZ() const { return _localZ; }
	std::vector<Transform*> children() const { return _children; }

	void setLocalPos(glm::vec2 pos);
	void setLocalRot(float rot);
	void setLocalScale(glm::vec2 scale);
	void setLocalZ(float z);

	void setParent(Transform* parent);

	glm::vec2 pos();
	float rot();
	glm::vec2 scale();
	float z() const;

	void setPos(glm::vec2 pos);
	void setRot(float rot);
	void setScale(glm::vec2 scale);
	void setZ(float z);

	glm::mat3 getMatrix();
	glm::mat3 getMatrixInv();

	void translate(const glm::vec2& v);
	void rotate(float degrees);

	void lookAt(const glm::vec2& target);

	glm::vec2 up();
	glm::vec2 down();
	glm::vec2 left();
	glm::vec2 right();

	glm::vec2 localToWorldPos(const glm::vec2& pos);
	glm::vec2 globalToLocalPos(const glm::vec2& pos);

	friend class GameObject;
	friend class Rigidbody;

	// DOTween
	Tween* doMove(const glm::vec2& endValue, float time);
	Tween* doRotate(float endValue, float time);
	Tween* doScale(const glm::vec2& endValue, float time);

	Tween* doLocalMove(const glm::vec2& endValue, float time);
	Tween* doLocalRotate(float endValue, float time);
	Tween* doLocalScale(const glm::vec2& endValue, float time);

	Tween* doMoveX(float endValue, float time);
	Tween* doMoveY(float endValue, float time);
	Tween* doLocalMoveX(float endValue, float time);
	Tween* doLocalMoveY(float endValue, float time);
};
