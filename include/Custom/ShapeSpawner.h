#pragma once
#include "Component.h"
#include "glm/vec2.hpp"

class ShapeSpawner : public Component
{
	float _spawnDelay = 0.05f;
	float _spawnTimer = 0;
	bool _enableGravity;
	bool _spawnOnClick;

	VectorDelayed<GameObjectSPtr> _spawnedShapes;

	explicit ShapeSpawner(GameObject* obj, bool enableGravity = false, bool spawnOnClick = true);

	void update() override;

public:
	GameObjectSPtr spawnSquare(glm::vec2 spawnPos, bool enableGravity = false);
	GameObjectSPtr spawnCircle(glm::vec2 spawnPos, bool enableGravity = false);

	friend class GameObject;
};
