#pragma once

#include "Component.h"

class Tank;
class Rigidbody;
class PolygonCollider;

class TankPlayerController : public Component
{
	float _gunRotSpeed = 180;

	Rigidbody* _rb = nullptr;
	bool _isMoving = false;

	Tank* _tank = nullptr;

	TankPlayerController(GameObject* obj, float moveSpeed = 100, float rotationSpeed = 60);

	void start() override;

	void update() override;
	void updateGunPosition() const;

	void fixedUpdate() override;

public:
	float moveSpeed;
	float rotationSpeed;

	bool isMoving() const;

	friend class GameObject;
};
