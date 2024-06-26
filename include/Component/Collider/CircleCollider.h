#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
	float _radius = 0;
	float _globalRadius = 0;

	std::optional<Collision> getCollisionWith(Collider* other) override;
	std::optional<Collision> getCollisionWith(PolygonCollider* other) override;
	std::optional<Collision> getCollisionWith(CircleCollider* other) override;

	std::vector<glm::vec2> findContactPoints(const Collider* other) const override;
	std::vector<glm::vec2> findContactPoints(const PolygonCollider* other) const override;
	std::vector<glm::vec2> findContactPoints(const CircleCollider* other) const override;

	bool isPointInside(const glm::vec2& point) const override;

	explicit CircleCollider(GameObject* obj, float radius = 1, bool isTrigger = false);

	void recalculate() override;

	float calculateMass() const override;
	float calculateInertia(float mass) const override;

	std::optional<Collision> getImpactCollision(glm::vec2 center, float radius) override;

public:
	float radius() const;
	void setRadius(float radius);

	friend class GameObject;
	friend class PolygonCollider;
};
