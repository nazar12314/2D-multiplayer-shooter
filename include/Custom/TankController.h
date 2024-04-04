#pragma once

#include "Component.h"

class Rigidbody;
class PolygonCollider;

class TankController : public Component
{
	Rigidbody* rb;

	TankController(Object* obj, float moveSpeed = 130, float rotationSpeed = 80);

	void fixedUpdate() override;

public:
	float moveSpeed;
	float rotationSpeed;

	friend class Object;
};
