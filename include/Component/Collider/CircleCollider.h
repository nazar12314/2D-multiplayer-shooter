#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
	float _radius = 0;

	std::optional<Collision> getCollisionWith(Collider* other) override;
	std::optional<Collision> getCollisionWith(PolygonCollider* other) override;
	std::optional<Collision> getCollisionWith(CircleCollider* other) override;

	std::vector<glm::vec2> findContactPoints(const Collider* other) const override;
	std::vector<glm::vec2> findContactPoints(const PolygonCollider* other) const override;
	std::vector<glm::vec2> findContactPoints(const CircleCollider* other) const override;

	explicit CircleCollider(Object* obj, float radius = 1, bool isTrigger = false);

	float calculateMass() const override;
	float calculateInertia(float mass) const override;

public:
	float radius() const { return _radius; }
	void setRadius(float radius) { _radius = radius; }

	friend class Object;
};
