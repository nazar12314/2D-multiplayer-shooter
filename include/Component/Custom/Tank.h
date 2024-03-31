#pragma once

#include "Component.h"

class Sprite;

class Tank : public Component
{
	float _gunRotSpeed;
	float _firerate;
	Sprite* gun;

	float shootCooldown;

	Tank(Object* obj, float gunRotSpeed = 180, float firerate = 1);
	void createGun();

	void update() override;
	void updateGunPosition() const;
	void shoot() const;

	friend class Object;
};
