#include <iostream>

#include "Collider.h"
#include "Physics.h"
#include "RigidBody.h"

RigidBody::RigidBody(Object* obj, float linearDrag, float angularDrag, float mass): Component(obj), linearDrag(linearDrag), angularDrag(angularDrag), mass(mass)
{
	recalculateAttachedCollider();
}

void RigidBody::setVelocity(glm::vec2 velocity)
{
	_velocity = velocity;
}
void RigidBody::addVelocity(glm::vec2 velocity)
{
	_velocity += velocity;
}
void RigidBody::setAngularVelocity(float angularVelocity)
{
	_angularVelocity = angularVelocity;
}
void RigidBody::addAngularVelocity(float angularVelocity)
{
	_angularVelocity += angularVelocity;
}

void RigidBody::collideWith(const std::vector<RigidBody*>& others) const
{
	for (auto& other : others)
	{
		if (attachedCollider->isTrigger() || other->attachedCollider->isTrigger()) continue;

		// Revert position because of collision
		attachedCollider->collisionEntered(other->attachedCollider);
		other->attachedCollider->collisionEntered(attachedCollider);

		attachedCollider->collisionStayed(other->attachedCollider);
		other->attachedCollider->collisionStayed(attachedCollider);

		attachedCollider->collisionExited(other->attachedCollider);
		other->attachedCollider->collisionExited(attachedCollider);

		std::cout << attachedCollider->obj->name() << " collided with " << other->attachedCollider->obj->name() << std::endl;
	}
}
void RigidBody::moveTo(glm::vec2 pos)
{
	if (attachedCollider == nullptr || attachedCollider->_isTrigger)
	{
		obj->setPos(pos);
		attachedCollider->recalculate();
		return;
	}

	auto oldPos = obj->pos();
	obj->setPos(pos);
	attachedCollider->recalculate();

	auto collisions = Physics::getCollisions(this);
	if (collisions.empty()) return;

	obj->setPos(oldPos);
	attachedCollider->recalculate();
	_velocity = glm::vec2(0);

	collideWith(collisions);
}
void RigidBody::rotateTo(float rot)
{
	if (attachedCollider == nullptr || attachedCollider->_isTrigger)
	{
		obj->setRot(rot);
		attachedCollider->recalculate();
		return;
	}

	auto oldRot = obj->rot();
	obj->setRot(rot);
	attachedCollider->recalculate();

	auto collisions = Physics::getCollisions(this);
	if (collisions.empty()) return;

	obj->setRot(oldRot);
	attachedCollider->recalculate();
	_angularVelocity = 0;

	collideWith(collisions);
}
Collider* RigidBody::recalculateAttachedCollider()
{
	if (attachedCollider == nullptr)
	{
		attachedCollider = obj->getComponent<Collider>();
		attachedCollider->recalculateOnTransformChange = false;
	}
	return attachedCollider;
}
