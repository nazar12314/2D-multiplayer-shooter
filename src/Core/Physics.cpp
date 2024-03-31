#include "Physics.h"

#include <iostream>

#include "Collider.h"
#include "MyTime.h"
#include "Rigidbody.h"
#include "glm/gtx/string_cast.hpp"

void Physics::init()
{
	Object::onComponentAddedGlobal += [](Component* comp)
	{
		if (auto rb = dynamic_cast<RigidBody*>(comp))
			rigidbodiesToAdd.push_back(rb);
	};
	Object::onComponentRemovedGlobal += [](Component* comp)
	{
		if (auto rb = dynamic_cast<RigidBody*>(comp))
			rigidbodiesToRemove.push_back(rb);
	};
}
void Physics::physicsLoop()
{
	float realTime = Time::getRealTime();
	while (realTime - lastFixedUpdateTime >= Time::fixedDeltaTime)
	{
		addOrRemoveRequestedRigidbodies();
		Time::fixedTick();
		updateCollisions();
		performFixedUpdate();

		lastFixedUpdateTime += Time::fixedDeltaTime;
	}
}

void Physics::performFixedUpdate()
{
	Object::fixedUpdateAll();

	for (auto& rb : rigidbodies)
	{
		if (length(rb->velocity()) > EPSILON)
		{
			rb->moveTo(rb->obj->pos() + rb->_velocity);
			rb->_velocity /= 1 + rb->linearDrag * Time::fixedDeltaTime;
		}
		else
			rb->_velocity = glm::vec2(0);

		if (glm::abs(rb->angularVelocity()) > EPSILON)
		{
			rb->rotateTo(rb->obj->rot() + rb->_angularVelocity);
			rb->_angularVelocity /= 1 + rb->angularDrag * Time::fixedDeltaTime;
		}
		else
			rb->_angularVelocity = 0;
	}
}

void Physics::updateCollisions()
{
	for (int i = 0; i < rigidbodies.size(); i++)
	{
		auto rb1 = rigidbodies[i];
		auto col1 = rb1->getAttachedCollider();
		if (col1 == nullptr) continue;

		for (int j = i + 1; j < rigidbodies.size(); j++)
		{
			auto rb2 = rigidbodies[j];
			auto col2 = rb2->getAttachedCollider();
			if (col2 == nullptr) continue;

			bool wasColliding = std::ranges::find(col1->collidingWith, col2) != col1->collidingWith.end();
			if (col1->collidesWith(col2))
			{
				std::cout << col1->obj->name() << " colliding with " << col2->obj->name() << std::endl;
				if (!wasColliding)
				{
					col1->collisionEntered(col2);
					col2->collisionEntered(col1);
				}
				col1->collisionStayed(col2);
				col2->collisionStayed(col1);
			}
			else
			{
				if (wasColliding)
				{
					col1->collisionExited(col2);
					col2->collisionExited(col1);
				}
			}
		}
	}
}
void Physics::addOrRemoveRequestedRigidbodies()
{
	for (auto& rb : rigidbodiesToAdd)
		rigidbodies.push_back(rb);
	rigidbodiesToAdd.clear();

	for (auto& rb : rigidbodiesToRemove)
		std::erase(rigidbodies, rb);
	rigidbodiesToRemove.clear();
}
