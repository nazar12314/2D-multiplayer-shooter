#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
	float _radius = 0;

	Collision getCollisionWith(Collider* other) override;
	Collision getCollisionWith(PolygonCollider* other) override;
	Collision getCollisionWith(CircleCollider* other) override;

	explicit CircleCollider(Object* obj, float radius = 1, bool isTrigger = false);

	float calculateInertia(float mass) const override;

public:
	float radius() const { return _radius; }
	void setRadius(float radius) { _radius = radius; }

	friend class Object;
};
