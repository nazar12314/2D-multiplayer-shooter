#pragma once

#include "Component.h"

class SpriteRenderer;

class Tank : public Component
{
	float _gunRotSpeed;
	float _firerate;
	SpriteRenderer* gun;

	float shootCooldown;

	Tank(Object* obj, float gunRotSpeed = 180, float firerate = 10);
	void createGun();

	void update() override;
	void updateGunPosition() const;
	void shoot() const;

	friend class Object;
};
