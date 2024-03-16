#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
public:
	float radius = 0;

	explicit CircleCollider(Object* obj) : Collider(obj) {}

    bool collidesWith(Collider *other) override;
};
