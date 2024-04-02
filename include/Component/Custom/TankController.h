#pragma once

#include "Component.h"

class Rigidbody;
class PolygonCollider;

class TankController : public Component
{
	Rigidbody* rb;

	TankController(Object* obj, float moveSpeed = 70, float rotationSpeed = 60);

	void fixedUpdate() override;

public:
	float moveSpeed;
	float rotationSpeed;

	friend class Object;
};
