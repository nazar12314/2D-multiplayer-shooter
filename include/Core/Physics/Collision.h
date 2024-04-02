#pragma once

#include "glm/vec2.hpp"

class Rigidbody;
class Collider;

struct Collision
{
	inline static const std::vector<glm::vec2> emptyContactPoints = {};

	bool collided;
	glm::vec2 norm;
	float depth;
	const std::vector<glm::vec2>& contactPoints;

	Collider* col1;
	Collider* col2;

	Collision(bool collided, const glm::vec2& norm, float depth, const std::vector<glm::vec2>& contactPoints, Collider* col1, Collider* col2) : collided(collided), norm(norm),
		depth(depth), contactPoints(contactPoints), col1(col1), col2(col2) {}
	Collision(bool collided) : collided(collided), norm(), depth(0), contactPoints(emptyContactPoints), col1(nullptr), col2(nullptr) {}
};
