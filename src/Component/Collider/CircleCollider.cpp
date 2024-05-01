#include "CircleCollider.h"

#include "MyMath.h"
#include "PolygonCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "glm/ext/scalar_constants.hpp"

CircleCollider::CircleCollider(GameObject* obj, float radius, bool isTrigger): Collider(obj, isTrigger), _radius(radius) {}
void CircleCollider::recalculate()
{
	_globalRadius = _radius * transform()->scale().x;
}

float CircleCollider::calculateMass() const
{
	return glm::pi<float>() * _globalRadius * _globalRadius;
}
float CircleCollider::calculateInertia(float mass) const
{
	return 0.5f * mass * _globalRadius * _globalRadius;
}
std::optional<Collision> CircleCollider::getImpactCollision(glm::vec2 center, float radius)
{
	auto [sep, norm] = Math::findMinSeparation(transform()->pos(), _globalRadius, center, radius);
	auto collided = sep <= 0;
	if (!collided) return std::nullopt;

	auto contactPoints = Math::findContactPoints(transform()->pos(), _globalRadius, center, radius);
	return Collision(norm, -sep, this, nullptr, contactPoints);
}

float CircleCollider::radius() const { return _radius; }
void CircleCollider::setRadius(float radius)
{
	_radius = radius;
	recalculate();
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
	auto [sep, norm] = Math::findMinSeparation(transform()->pos(), _globalRadius, other->transform()->pos(), other->_globalRadius);
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
	return Math::findContactPoints(transform()->pos(), _globalRadius, other->transform()->pos(), other->_globalRadius);
}

bool CircleCollider::isPointInside(const glm::vec2& point) const
{
	return distance(transform()->pos(), point) <= _globalRadius;
}
