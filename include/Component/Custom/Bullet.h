#pragma once

#include "Component.h"

class Collider;
class RigidBody;

class Bullet : public Component
{
	float _speed;
	RigidBody* rb;

	Bullet(Object* obj, float speed = 12);

	void fixedUpdate() override;
	void onTriggerEnter(Collider* other) override;

public:
	friend class Object;
};
