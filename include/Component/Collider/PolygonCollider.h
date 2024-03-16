#pragma once

#include "Object.h"
#include "Collider.h"
#include "glm/vec2.hpp"
#include <vector>
#include "glm/gtx/rotate_vector.hpp"
#include <iostream>

class PolygonCollider : public Collider
{
	void lateUpdate() override;

public:
	glm::vec2 size {0, 0};
	std::vector<glm::vec2> edges;

	PolygonCollider(Object* obj, glm::vec2 size) : Collider(obj), size(size)
	{
		updateEdges();
	}

	bool collidesWith(Collider* other) override;
	static double findDistance(double startX, double startY, double endX, double endY);

public:
	void updateEdges()
	{
		glm::vec2 sPos = obj->pos();
		float angle = glm::radians(obj->rot());

		edges = {
			{sPos.x - size.x / 2, sPos.y - size.y / 2}, // bottom left
			{sPos.x - size.x / 2, sPos.y + size.y / 2}, // top left
			{sPos.x + size.x / 2, sPos.y + size.y / 2}, // top right
			{sPos.x + size.x / 2, sPos.y - size.y / 2}  // bottom right
		};

		for (auto& edge : edges)
		{
			edge = glm::rotate(edge - sPos, angle) + sPos;
		}
	}
};
