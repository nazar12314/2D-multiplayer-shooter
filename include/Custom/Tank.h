#pragma once

#include "Component.h"

class SpriteRenderer;

class Tank : public Component
{
	float _gunRotSpeed = 180;
	float _firerate = 4;
	SpriteRenderer* gun;
	bool _explosiveBullets;

	float shootCooldown;

	Tank(Object* obj, bool explosiveBullets = false);
	void createGun();

	void update() override;
	void updateGunPosition() const;
	void shoot() const;

	friend class Object;
};
