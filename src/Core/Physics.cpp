#include "Physics.h"

#include <iostream>

#include "MyTime.h"
#include "Rigidbody.h"
#include "glm/gtx/string_cast.hpp"

void Physics::init()
{
	Object::onComponentAdded += [](Component* comp)
	{
		if (auto rb = dynamic_cast<RigidBody*>(comp))
			rigidbodies.push_back(rb);
	};
}
void Physics::physicsLoop()
{
	float realTime = Time::getRealTime();
	while (realTime - lastFixedUpdateTime >= Time::fixedDeltaTime)
	{
		Time::fixedTick();
		fixedUpdate();

		lastFixedUpdateTime += Time::fixedDeltaTime;
	}
}

void Physics::fixedUpdate()
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
