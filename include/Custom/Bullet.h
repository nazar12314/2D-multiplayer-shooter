#pragma once

#include "Component.h"

class Collider;
class Rigidbody;

class Bullet : public Component
{
	float _speed;
	Rigidbody* rb;
	bool _explode;

	Bullet(Object* obj, float speed = 16, bool explode = false);

	void fixedUpdate() override;
	void onTriggerEnter(Collider* other) override;

public:
	friend class Object;
};
