#pragma once

#include "Component.h"

class Tank;
class Rigidbody;

class TankRemoteController : public Component
{
	float _gunRotSpeed = 180;
	float _smoothness = 5.0f;

	Rigidbody* _rb = nullptr;
	Tank* _tank = nullptr;

	float _requestedMovement = 0;
	float _requestedRotation = 0;
	float _requestedGunTargetRotation = 0;
	bool _requestShoot = false;

	float _targetGunRotation = 0;

	TankRemoteController(GameObject* obj);

	void start() override;

	void updatePos(const glm::vec2& pos) const;
	void updateRot(float rot) const;
	void updateGunRot(float rot) const;
	void updateShoot(bool shoot) const;

	void applyMovement(float movement) const;
	void applyRotation(float rotation) const;
	void applyGunRotation(float rotation);
	void applyShoot(bool shoot) const;

	void fixedUpdate() override;
	void updateGunRotation() const;

public:
	void requestMove(float movement);
	void requestRotate(float rotation);
	void requestGunTargetRotation(float rotation);
	void requestShoot();

	float getAndResetRequestedMovement();
	float getAndResetRequestedRotation();
	float getAndResetRequestedGunRotation();
	bool getAndResetRequestedShoot();

	friend class GameObject;
	friend class Multiplayer;
};
