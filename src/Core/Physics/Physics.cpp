#include "Physics.h"

#include <algorithm>

#include "Collider.h"
#include "Gizmos.h"
#include "MyTime.h"
#include "Rigidbody.h"
#include "Solver.h"

void Physics::init()
{
	subscribeToEvents();

	solvers.push_back(new PositionSolver());
	solvers.push_back(new ImpulseSolver());
}
void Physics::subscribeToEvents()
{
	Object::onComponentAddedGlobal += [](Component* comp)
	{
		if (auto rb = dynamic_cast<Rigidbody*>(comp))
			rigidbodies.push_back_delayed(rb);
	};
	Object::onComponentRemovedGlobal += [](Component* comp)
	{
		if (auto rb = dynamic_cast<Rigidbody*>(comp))
			rigidbodies.erase_delayed(rb);
	};
}

void Physics::physicsLoop()
{
	float realTime = Time::getRealTime();
	while (realTime - lastFixedUpdateTime >= Time::fixedDeltaTime)
	{
		Time::fixedTick();
		lastFixedUpdateTime += Time::fixedDeltaTime;

		// !!! PHYSICS STEP !!!
		step(Time::fixedDeltaTime);
		// !!! PHYSICS STEP !!!

		Object::fixedUpdateAll();
	}
}

void Physics::step(float dt)
{
	// Simulate rigidbodies
	rigidbodies.apply_changes();
	for (auto& rb : rigidbodies)
		rb->step(dt);

	solveCollisions();
}

void Physics::solveCollisions()
{
	auto [collisions, triggers] = findCollisions();

	//for (auto& col : collisions)
	//	for (auto& point : col.contactPoints)
	//		Gizmos::drawPoint(point, 0.05f, Color::red);

	// Resolve collisions
	for (auto& solver : solvers)
		solver->solveCollisions(collisions);

	sendCollisionCallbacks(collisions);
	sendTriggerCallbacks(triggers);
}


std::pair<std::vector<Collision>, std::vector<Collision>> Physics::findCollisions()
{
	std::vector<Collision> collisions;
	std::vector<Collision> triggers;

	for (int i = 0; i < rigidbodies.size(); i++)
	{
		auto rb1 = rigidbodies[i];
		auto col1 = rb1->_attachedCollider;

		for (int j = i + 1; j < rigidbodies.size(); j++)
		{
			auto rb2 = rigidbodies[j];
			auto col2 = rb2->_attachedCollider;
			bool isTrigger = col1->_isTrigger || col2->_isTrigger;

			auto collision = col1->getCollisionWith(col2);
			if (!collision.collided) continue;

			if (isTrigger)
				triggers.push_back(collision);
			else
				collisions.push_back(collision);
		}
	}
	return {collisions, triggers};
}

void Physics::sendCollisionCallbacks(const std::vector<Collision>& collisions)
{
	for (auto& collision : collisions)
	{
		auto col1 = collision.col1;
		auto col2 = collision.col2;

		bool currentlyColliding = std::ranges::find(col1->collidingWith, col2) != col1->collidingWith.end();
		if (collision.collided)
		{
			if (!currentlyColliding)
			{
				col1->collisionEntered(col2);
				col2->collisionEntered(col1);
			}
			col1->collisionStayed(col2);
			col2->collisionStayed(col1);
		}
		else
		{
			if (currentlyColliding)
			{
				col1->collisionExited(col2);
				col2->collisionExited(col1);
			}
		}
	}
}
void Physics::sendTriggerCallbacks(const std::vector<Collision>& triggers)
{
	for (auto& trigger : triggers)
	{
		auto col1 = trigger.col1;
		auto col2 = trigger.col2;

		bool currentlyTriggering = std::ranges::find(col1->triggeringWith, col2) != col1->triggeringWith.end();
		if (trigger.collided)
		{
			if (!currentlyTriggering)
			{
				col1->triggerEntered(col2);
				col2->triggerEntered(col1);
			}
			col1->triggerStayed(col2);
			col2->triggerStayed(col1);
		}
		else
		{
			if (currentlyTriggering)
			{
				col1->triggerExited(col2);
				col2->triggerExited(col1);
			}
		}
	}
}
