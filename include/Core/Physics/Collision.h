#pragma once

#include "glm/vec2.hpp"

class Rigidbody;
class Collider;

struct Collision
{
	glm::vec2 norm;
	float depth;
	std::vector<glm::vec2> contactPoints;

	Collider* col1;
	Collider* col2;

	Collision(const glm::vec2& norm, float depth, Collider* col1, Collider* col2) : norm(norm), depth(depth), col1(col1), col2(col2) {}
	Collision(const glm::vec2& norm, float depth, Collider* col1, Collider* col2, const std::vector<glm::vec2>& points) : norm(norm), depth(depth), contactPoints(points),
		col1(col1), col2(col2) {}

	void setContactPoints(const std::vector<glm::vec2>& points) { contactPoints = points; }
};
