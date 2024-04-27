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

	Tank(GameObject* obj, bool explosiveBullets = false, bool explodeAtMousePosition = false);
	void createGun();
	void createParts();
	void createParticles() const;

	void update() override;
	void updateGunPosition() const;
	void shoot() const;

	friend class GameObject;
};
