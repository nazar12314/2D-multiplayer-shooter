#pragma once

#include "Collider.h"
#include "glm/vec2.hpp"

class SpriteRenderer;
class Rigidbody;

class PolygonCollider : public Collider
{
	constexpr static bool DISPLAY_VERTICES_DEBUG = false;

protected:
	std::vector<glm::vec2> _vertices {};
	std::vector<glm::vec2> _globalVertices {};

	std::vector<SpriteRenderer*> vertexSpritesTEST {};

	PolygonCollider(GameObject* obj, const std::vector<glm::vec2>& vertices, bool isTrigger = false);

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
	void setVertices(const std::vector<glm::vec2>& vertices);

	friend class GameObject;
	friend class CircleCollider;
};

class BoxCollider : public PolygonCollider
{
	glm::vec2 _size;

	static std::vector<glm::vec2> calculateBoxVertices(glm::vec2 size);

public:
	BoxCollider(GameObject* obj, glm::vec2 size = {1, 1}, bool isTrigger = false);

	glm::vec2 size() const;
	void setSize(glm::vec2 size);

	friend class GameObject;
};
