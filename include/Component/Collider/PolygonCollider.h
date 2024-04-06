#pragma once

#include "Collider.h"
#include "glm/vec2.hpp"

class SpriteRenderer;
class Rigidbody;

class PolygonCollider : public Collider
{
	constexpr static bool DISPLAY_VERTICES_DEBUG = false;

	glm::vec2 _size {0, 0};
	std::vector<glm::vec2> _vertices {};
	std::vector<glm::vec2> _globalVertices {};

	std::vector<SpriteRenderer*> vertexSpritesTEST {};

	PolygonCollider(GameObject* obj, glm::vec2 size, bool isTrigger = false);
	void updateVertices();
	void recalculate() override;

	std::optional<Collision> getCollisionWith(Collider* other) override;
	std::optional<Collision> getCollisionWith(PolygonCollider* other) override;
	std::optional<Collision> getCollisionWith(CircleCollider* other) override;

	std::vector<glm::vec2> findContactPoints(const Collider* other) const override;
	std::vector<glm::vec2> findContactPoints(const PolygonCollider* other) const override;
	std::vector<glm::vec2> findContactPoints(const CircleCollider* other) const override;

	bool isPointInside(const glm::vec2& point) const override;
	std::optional<Collision> getImpactCollision(glm::vec2 center, float radius) override;

	void onDestroy() override;

	float calculateMass() const override;
	float calculateInertia(float mass) const override;

public:
	void size(glm::vec2 size);
	void setSize(glm::vec2 size);

	friend class GameObject;
	friend class CircleCollider;
};
