#pragma once

#include "Component.h"

class Collider;
class Rigidbody;

class Bullet : public Component
{
	float _speed;
	Rigidbody* rb;
	bool _explode;

	Collider* _colliderToIgnore;

	Bullet(GameObject* obj, Collider* colliderToIgnore = nullptr, float speed = 25, bool explode = false);

	void fixedUpdate() override;
	void onTriggerEnter(Collider* other) override;

	friend class GameObject;
};
