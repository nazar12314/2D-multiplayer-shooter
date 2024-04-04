#include "Collider.h"

#include "Object.h"
#include "Rigidbody.h"

Collider::Collider(Object* obj, bool isTrigger): Component(obj), _isTrigger(isTrigger)
{
	if (obj->tryGetComponent<Rigidbody>(_rb))
		_rb->attachCollider(this);
}
Collider::~Collider()
{
	if (_rb != nullptr)
		_rb->resetCollider();
}

void Collider::start()
{
	recalculate();
}

bool Collider::isTrigger() const { return _isTrigger; }
void Collider::setIsTrigger(bool trigger) { _isTrigger = trigger; }

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
