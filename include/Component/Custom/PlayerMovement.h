#pragma once
#include "Component.h"

class PolygonCollider;

class PlayerMovement : public Component
{
	explicit PlayerMovement(Object* obj, float speed = 10) : Component(obj),speed(speed) {}

	void update() override;

public:
	float speed;
	PolygonCollider* col = nullptr;

	friend class Object;
};
