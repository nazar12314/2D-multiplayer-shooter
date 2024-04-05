#pragma once

#include "Component.h"

class SpriteRenderer;

class Tank : public Component
{
	float _gunRotSpeed = 180;
	float _firerate = 4;
	bool _explosiveBullets;
	bool _explodeAtMousePosition;

	SpriteRenderer* gun;
	Transform* gunPivot;

	float _shootCooldown;

	Tank(Object* obj, bool explosiveBullets = false, bool explodeAtMousePosition = false);
	void createGun();

	void update() override;
	void updateGunPosition() const;
	void shoot() const;

	friend class Object;
};
