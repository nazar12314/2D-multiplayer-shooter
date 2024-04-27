#pragma once

#include "Component.h"

class Rigidbody;
class PolygonCollider;

class TankController : public Component
{
	Rigidbody* rb = nullptr;
	bool _isMoving = false;

	TankController(GameObject* obj, float moveSpeed = 100, float rotationSpeed = 60);

	void fixedUpdate() override;

public:
	float moveSpeed;
	float rotationSpeed;

	bool isMoving() const;

	friend class GameObject;
};
