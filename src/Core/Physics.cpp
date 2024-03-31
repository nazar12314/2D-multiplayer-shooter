#include "Physics.h"

#include <iostream>
#include <algorithm>
#include "Collider.h"
#include "MyTime.h"
#include "Rigidbody.h"
#include "glm/gtx/string_cast.hpp"

void Physics::init()
{
	Object::onComponentAddedGlobal += [](Component* comp)
	{
		if (auto rb = dynamic_cast<RigidBody*>(comp))
			rigidbodies.push_back_delayed(rb);
	};
	Object::onComponentRemovedGlobal += [](Component* comp)
	{
		if (auto rb = dynamic_cast<RigidBody*>(comp))
			rigidbodies.erase_delayed(rb);
	};
}
void Physics::physicsLoop()
{
	float realTime = Time::getRealTime();
	while (realTime - lastFixedUpdateTime >= Time::fixedDeltaTime)
	{
		Time::fixedTick();
		updatePhysics();
		updateTriggers();
		Object::fixedUpdateAll();

		lastFixedUpdateTime += Time::fixedDeltaTime;
	}
}

void Physics::updatePhysics()
{
	rigidbodies.apply_changes();
	for (auto& rb : rigidbodies)
		rb->recalculateAttachedCollider();

	for (auto& rb : rigidbodies)
	{
		if (length(rb->velocity()) > EPSILON)
		{
			auto finalVelocity = rb->_velocity / (1 + rb->linearDrag * Time::fixedDeltaTime);
			rb->moveTo(rb->obj->pos() + (rb->_velocity + finalVelocity) / 2.0f);
			rb->_velocity /= 1 + rb->linearDrag * Time::fixedDeltaTime;
		}
		else
			rb->_velocity = glm::vec2(0);

		if (glm::abs(rb->angularVelocity()) > EPSILON)
		{
			auto finalAngularVelocity = rb->_angularVelocity / (1 + rb->angularDrag * Time::fixedDeltaTime);
			rb->rotateTo(rb->obj->rot() + (rb->_angularVelocity + finalAngularVelocity) / 2.0f);
			rb->_angularVelocity /= 1 + rb->angularDrag * Time::fixedDeltaTime;
		}
		else
			rb->_angularVelocity = 0;
	}
}

void Physics::updateTriggers()
{
	for (int i = 0; i < rigidbodies.size(); i++)
	{
		auto rb1 = rigidbodies[i];
		auto col1 = rb1->attachedCollider;
		if (col1 == nullptr) continue;

		for (int j = i + 1; j < rigidbodies.size(); j++)
		{
			auto rb2 = rigidbodies[j];
			auto col2 = rb2->attachedCollider;
			if (col2 == nullptr || (!col1->_isTrigger && !col2->_isTrigger)) continue;

			updateTriggering(col1, col2, col1->intersectsWith(col2));
		}
	}
}
void Physics::updateTriggering(Collider* col1, Collider* col2, bool intersecting)
{
	bool isTriggering = std::ranges::find(col1->triggeringWith, col2) != col1->triggeringWith.end();
	if (intersecting)
	{
		std::cout << col1->obj->name() << " triggered with " << col2->obj->name() << std::endl;
		if (!isTriggering)
		{
			col1->triggerEntered(col2);
			col2->triggerEntered(col1);
		}
		col1->triggerStayed(col2);
		col2->triggerStayed(col1);
	}
	else
	{
		if (isTriggering)
		{
			col1->triggerExited(col2);
			col2->triggerExited(col1);
		}
	}
}

std::vector<RigidBody*> Physics::getCollisions(const RigidBody* rb)
{
	if (rb->attachedCollider == nullptr || rb->attachedCollider->_isTrigger) return {};

	std::vector<RigidBody*> collisions {};
	for (auto& other : rigidbodies)
	{
		if (other == rb) continue;
		if (other->attachedCollider == nullptr || other->attachedCollider->_isTrigger) continue;

		if (rb->attachedCollider->intersectsWith(other->attachedCollider))
			collisions.push_back(other);
	}
	return collisions;
}
