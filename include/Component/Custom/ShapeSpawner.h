#pragma once
#include "Component.h"

class ShapeSpawner : public Component
{
	float spawnDelay = 0.05f;
	float _spawnTimer = 0;

	explicit ShapeSpawner(Object* obj) : Component(obj) {}

	void update() override;

	friend class Object;
};
