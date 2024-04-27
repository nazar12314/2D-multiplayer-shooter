#pragma once

#include "Component.h"

class TankController;

class TankEffects : public Component
{
	float _particlesPerDistance = 0.3f;

	float _movedByTracker = 0;
	glm::vec2 _lastPos = {0, 0};
	TankController* _tankController = nullptr;

	explicit TankEffects(GameObject* gameObject) : Component(gameObject) {}

	void start() override;
	void update() override;

	void spawnSmokeParticle() const;

	friend class GameObject;
};
