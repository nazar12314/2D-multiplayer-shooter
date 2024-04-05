#pragma once
#include "Component.h"
#include "glm/vec2.hpp"

class ShapeSpawner : public Component
{
	float spawnDelay = 0.05f;
	float _spawnTimer = 0;
	bool enableGravity;

	explicit ShapeSpawner(Object* obj, bool enableGravity = false) : Component(obj), enableGravity(enableGravity) {}

	void update() override;

public:
	static Object* spawnSquare(glm::vec2 spawnPos, bool enableGravity = false);
	static Object* spawnCircle(glm::vec2 spawnPos, bool enableGravity = false);

	friend class Object;
};
