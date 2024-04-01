#pragma once

#include "Component.h"

class RigidBody;
class PolygonCollider;

class TankController : public Component
{
	RigidBody* rb;

	TankController(Object* obj, float moveSpeed = 1, float rotationSpeed = 45);

	void fixedUpdate() override;

public:
	float moveSpeed;
	float rotationSpeed;

	friend class Object;
};
