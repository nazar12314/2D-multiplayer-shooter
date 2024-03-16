#pragma once

#include <bits/stl_pair.h>

#include "Collider.h"
#include "glm/vec2.hpp"

class BoxCollider : public Collider
{
public:
	glm::vec2 size {0, 0};

	explicit BoxCollider(Object* obj) : Collider(obj) {}

	bool collidesWith(const Collider* other) override;
	static double findDistance(double startX, double startY, double endX, double endY);
	static std::pair<glm::vec2, glm::vec2> getAxis(const BoxCollider* box);

	static float projectOntoAxis(const glm::vec2& pos, const glm::vec2& axisX, const glm::vec2& axisY, const glm::vec2& halfSize);
	bool overlapOnAxis(const glm::vec2& sPos, const glm::vec2& oPos, const glm::vec2& axisX, const glm::vec2& axisY, const BoxCollider* box) const;
};
