#pragma once
#include "Component.h"

class RigidBody;

class Bullet : public Component
{
	float _speed;
	RigidBody* rb;

	Bullet(Object* obj, float speed = 12);

	void fixedUpdate() override;

public:
	friend class Object;
};
