#pragma once

#include "glm/vec2.hpp"

class Rigidbody;
class Collider;

struct Collision
{
	bool collided;
	glm::vec2 norm;
	float depth;
	std::vector<glm::vec2> contactPoints;

	Collider* col1;
	Collider* col2;

	Collision(bool collided, const glm::vec2& norm, float depth, const std::vector<glm::vec2>& contactPoints, Collider* col1, Collider* col2) : collided(collided), norm(norm), depth(depth), contactPoints(contactPoints), col1(col1), col2(col2){}
};
