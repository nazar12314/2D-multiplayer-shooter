#pragma once

#include "Component.h"

class TankRemoteController;
class SpriteRenderer;

class Tank : public Component
{
	float _firerate = 4;

	bool _controlledByPlayer;

	SpriteRenderer* _gun = nullptr;
	Transform* _gunPivot = nullptr;

	float _shootTimer = 0;

	Tank(GameObject* obj, bool controlledByPlayer);

	void start() override;
	void createGun();
	void createParts();
	void createParticles();

	void update() override;
	void shoot() const;

	friend class GameObject;
	friend class TankRemoteController;
	friend class TankPlayerController;
};
