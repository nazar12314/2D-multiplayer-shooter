#pragma once

#include "Object.h"
#include "Collider.h"
#include "glm/vec2.hpp"
#include <vector>
#include "glm/gtx/rotate_vector.hpp"

class PolygonCollider : public Collider
{
	glm::vec2 _size {0, 0};
	std::vector<glm::vec2> _edges;

	PolygonCollider(Object* obj, glm::vec2 size);

	void lateUpdate() override;

public:
	bool collidesWith(Collider* other) override;

	void updateEdges();

	friend class Object;
};
