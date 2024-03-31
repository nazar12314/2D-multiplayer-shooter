#include "Collider.h"

#include "Object.h"

Collider::Collider(Object* obj, bool isTrigger): Component(obj), _isTrigger(isTrigger) {}

void Collider::start()
{
	recalculate();
}

bool Collider::isTrigger() const { return _isTrigger; }
void Collider::setIsTrigger(bool trigger) { _isTrigger = trigger; }

void Collider::lateUpdate()
{
	if (!obj->transformChanged || !recalculateOnTransformChange) return;
	recalculate();
}

bool Collider::intersectsWith(Collider* other) { return false; }

void Collider::collisionEntered(Collider* other) const
{
	obj->onCollisionEnter(other);
}
void Collider::collisionStayed(Collider* other) const
{
	obj->onCollisionStay(other);
}
void Collider::collisionExited(Collider* other) const
{
	obj->onCollisionExit(other);
}

void Collider::triggerEntered(Collider* other)
{
	triggeringWith.push_back(other);
	obj->onTriggerEnter(other);
}
void Collider::triggerStayed(Collider* other) const
{
	obj->onTriggerStay(other);
}
void Collider::triggerExited(Collider* other)
{
	std::erase(triggeringWith, other);
	obj->onTriggerExit(other);
}
