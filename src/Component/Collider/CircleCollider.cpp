#include "CircleCollider.h"

#include "MyMath.h"
#include "PolygonCollider.h"
#include "Object.h"
#include "glm/ext/scalar_constants.hpp"

CircleCollider::CircleCollider(Object* obj, float radius, bool isTrigger): Collider(obj, isTrigger), _radius(radius) {}
float CircleCollider::calculateMass() const
{
	return glm::pi<float>() * _radius * _radius;
}
float CircleCollider::calculateInertia(float mass) const
{
	return 0.5f * mass * _radius * _radius;
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
	auto [sep, norm] = Math::findMinSeparation(obj->pos(), _radius, other->obj->pos(), other->_radius);
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
	return Math::findContactPoints(obj->pos(), _radius, other->obj->pos(), other->_radius);
}
