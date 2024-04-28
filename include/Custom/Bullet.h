#pragma once

#include "Component.h"

class Collider;
class Rigidbody;

class Bullet : public Component
{
	float _speed;
	Rigidbody* rb;
	bool _explode;

	float _immuneTimer = 0.02f;

	Bullet(GameObject* obj, float speed = 25, bool explode = false);

	void fixedUpdate() override;
	void onTriggerEnter(Collider* other) override;

	friend class GameObject;
};
