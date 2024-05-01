#pragma once

#include "Component.h"

class TankRemoteController;
class Tank;
class Rigidbody;
class PolygonCollider;

class TankPlayerController : public Component
{
	float _moveSpeed = 50;
	float _rotationSpeed = 30;

	TankRemoteController* _remoteController = nullptr;
	Rigidbody* _rb = nullptr;

	Tank* _tank = nullptr;

	TankPlayerController(GameObject* obj, TankRemoteController* remoteController);

	void start() override;

	void fixedUpdate() override;
	void updateMovement() const;
	void updateGunRotation() const;

	friend class GameObject;
};
