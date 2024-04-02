#include "CircleCollider.h"

#include "MyMath.h"
#include "PolygonCollider.h"
#include "Object.h"

CircleCollider::CircleCollider(Object* obj, float radius, bool isTrigger): Collider(obj, isTrigger), _radius(radius) {}
float CircleCollider::calculateInertia(float mass) const
{
	return 0.5f * mass * _radius * _radius;
}

Collision CircleCollider::getCollisionWith(Collider* other)
{
	return other->getCollisionWith(this);
}
Collision CircleCollider::getCollisionWith(PolygonCollider* other)
{
	return other->getCollisionWith(this);
}
Collision CircleCollider::getCollisionWith(CircleCollider* other)
{
	auto [sep, norm] = Math::findMinSeparation(obj->pos(), _radius, other->obj->pos(), other->_radius);
	auto collided = sep < 0;
	if (!collided) return Collision(false);

	auto contactPoints = Math::findContactPoints(obj->pos(), _radius, other->obj->pos(), other->_radius);
	return Collision(collided, norm, -sep, contactPoints, this, other);
}
