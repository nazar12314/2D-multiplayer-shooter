#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "glm/geometric.hpp"
#include "Object.h"

CircleCollider::CircleCollider(Object* obj, bool isTrigger): Collider(obj, isTrigger) {}

bool CircleCollider::intersectsWith(Collider* other)
{
	if (auto* box = dynamic_cast<PolygonCollider*>(other))
		return box->intersectsWith(this);

	if (auto* circle = dynamic_cast<CircleCollider*>(other))
	{
		float distance = glm::distance(obj->pos(), other->obj->pos());
		return distance < radius + circle->radius;
	}

	return false;
}
