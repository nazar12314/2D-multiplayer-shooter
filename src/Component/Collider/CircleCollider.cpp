#include "CircleCollider.h"

#include "MyMath.h"
#include "PolygonCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "glm/ext/scalar_constants.hpp"

CircleCollider::CircleCollider(GameObject* obj, float radius, bool isTrigger): Collider(obj, isTrigger), _radius(radius) {}
float CircleCollider::calculateMass() const
{
	return glm::pi<float>() * _radius * _radius;
}
float CircleCollider::calculateInertia(float mass) const
{
	return 0.5f * mass * _radius * _radius;
}
std::optional<Collision> CircleCollider::getImpactCollision(glm::vec2 center, float radius)
{
	auto [sep, norm] = Math::findMinSeparation(transform()->getPos(), _radius, center, radius);
	auto collided = sep <= 0;
	if (!collided) return std::nullopt;

	auto contactPoints = Math::findContactPoints(transform()->getPos(), _radius, center, radius);
	return Collision(norm, -sep, this, nullptr, contactPoints);
}

std::optional<Collision> CircleCollider::getCollisionWith(Collider* other)
{
	return other->getCollisionWith(this);
}
std::optional<Collision> CircleCollider::getCollisionWith(PolygonCollider* other)
{
	return other->getCollisionWith(this);
}
std::optional<Collision> CircleCollider::getCollisionWith(CircleCollider* other)
{
	auto [sep, norm] = Math::findMinSeparation(transform()->getPos(), _radius, other->transform()->getPos(), other->_radius);
	auto collided = sep <= 0;
	if (!collided) return std::nullopt;

	return Collision(norm, -sep, this, other);
}

std::vector<glm::vec2> CircleCollider::findContactPoints(const Collider* other) const
{
	return other->findContactPoints(this);
}
std::vector<glm::vec2> CircleCollider::findContactPoints(const PolygonCollider* other) const
{
	return other->findContactPoints(this);
}
std::vector<glm::vec2> CircleCollider::findContactPoints(const CircleCollider* other) const
{
	return Math::findContactPoints(transform()->getPos(), _radius, other->transform()->getPos(), other->_radius);
}

bool CircleCollider::isPointInside(const glm::vec2& point) const
{
	return distance(transform()->getPos(), point) <= _radius;
}
