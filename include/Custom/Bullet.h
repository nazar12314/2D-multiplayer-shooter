#pragma once

#include "Component.h"

class Tank;
class Collider;
class Rigidbody;

class Bullet : public Component
{
	float _speed;
	bool _explode;
	Tank* _shooter;
	Rigidbody* _rb;

	Collider* _colliderToIgnore;

	Bullet(GameObject* obj, Tank* shooter, Collider* colliderToIgnore = nullptr, float speed = 25, bool explode = false);

	void fixedUpdate() override;
	void onTriggerEnter(Collider* other) override;

	friend class GameObject;
};
