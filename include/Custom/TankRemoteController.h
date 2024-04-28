#pragma once

#include "Component.h"

class Tank;
class Rigidbody;

class TankRemoteController : public Component
{
	Rigidbody* _rb = nullptr;
	Tank* _tank = nullptr;

	TankRemoteController(GameObject* obj);

	void start() override;

public:
	void moveTo(const glm::vec2& pos) const;
	void rotateGunTo(float angle) const;

	friend class GameObject;
};
