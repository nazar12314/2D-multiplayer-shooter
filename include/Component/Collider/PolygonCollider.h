#pragma once

#include "Object.h"
#include "Collider.h"
#include "glm/vec2.hpp"
#include <vector>
#include "glm/gtx/rotate_vector.hpp"

class SpriteRenderer;

class PolygonCollider : public Collider
{
	glm::vec2 _size {0, 0};
	std::vector<glm::vec2> _edges;

	PolygonCollider(Object* obj, glm::vec2 size, bool isTrigger = false);

	std::vector<SpriteRenderer*> edgePointsTEMP {};

public:
	bool intersectsWith(Collider* other) override;

	void recalculate() override;

	void onDestroy() override;

	friend class Object;
};
