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
	std::vector<glm::vec2> _globalVerticesStorage {};

	std::vector<SpriteRenderer*> vertexSpritesTEST {};

	PolygonCollider(Object* obj, glm::vec2 size, bool isTrigger = false);
	void updateVertices();

	Collision getCollisionWith(Collider* other) override;
	Collision getCollisionWith(PolygonCollider* other) override;
	Collision getCollisionWith(CircleCollider* other) override;

	std::vector<glm::vec2>& calculateGlobalVertices();

	void onDestroy() override;

	float calculateInertia(float mass) const override;

public:
	void size(glm::vec2 size);
	void setSize(glm::vec2 size);

	friend class Object;
	friend class CircleCollider;
};
