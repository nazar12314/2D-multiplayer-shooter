#include "Collider.h"

#include "Object.h"

Collider::Collider(Object* obj): Component(obj) {}

void Collider::collisionEntered(Collider* other)
{
	collidingWith.push_back(other);
	obj->onCollisionEnter(other);
}
void Collider::collisionStayed(Collider* other) const
{
	obj->onCollisionStay(other);
}
void Collider::collisionExited(Collider* other)
{
	std::erase(collidingWith, other);
	obj->onCollisionExit(other);
}
