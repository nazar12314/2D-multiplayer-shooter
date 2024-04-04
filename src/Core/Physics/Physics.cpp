#include "Physics.h"

#include <execution>
#include <iostream>

#include "Collider.h"
#include "Gizmos.h"
#include "Input.h"
#include "MyTime.h"
#include "Rigidbody.h"
#include "Solver.h"
#include "Utils.h"

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
	if constexpr (MANUAL_UPDATE)
	{
		if (Input::wasKeyPressed(SDLK_SPACE))
			step(Time::fixedDeltaTime, 1);
		if (Input::isKeyDown(SDLK_SPACE) && Input::isKeyDown(SDLK_LCTRL))
			step(Time::fixedDeltaTime, 1);
		return;
	}

	fixedUpdateTimer -= Time::deltaTime;
	while (fixedUpdateTimer <= 0)
	{
		Time::fixedTick();
		fixedUpdateTimer += Time::fixedDeltaTime;
		Object::fixedUpdateAll();

		// !!! PHYSICS STEP !!!
		step(Time::fixedDeltaTime);
		// !!! PHYSICS STEP !!!
	}
}

void Physics::step(float dt, int substeps)
{
	rigidbodies.apply_changes();
	for (int i = 0; i < substeps; i++)
	{
		for (auto& rb : rigidbodies)
			rb->step(dt / SUBSTEPS);

		solveCollisions();

		displayContactPoints_debug();
	}
}

void Physics::displayContactPoints_debug()
{
	if constexpr (!DISPLAY_CONTACT_POINTS) return;

	for (auto& gizmo : gizmos)
		Gizmos::remove(gizmo);
	gizmos.clear();

	for (auto& col : collisionStorage)
		for (auto& point : col.contactPoints)
			gizmos.push_back(Gizmos::drawPoint(point, 0.05f, Color::red, 999));
	for (auto& trig : triggerStorage)
		for (auto& point : trig.contactPoints)
			gizmos.push_back(Gizmos::drawPoint(point, 0.05f, Color::green, 999));
}

void Physics::solveCollisions()
{
	//static int n = 0;
	//static int seqSum = 0;
	//static int parSum = 0;

	//seqSum += Utils::measureTime(updateCollisionsSequential);
	//parSum += Utils::measureTime(updateCollisionsParallel);

	//n++;
	//std::cout << seqSum / n << " " << parSum / n << " " << seqSum / (float)parSum << std::endl;

	updateCollisionsParallel();

	// Resolve collisions
	for (auto& solver : solvers)
		solver->solveCollisions(collisionStorage);

	sendCollisionCallbacks(collisionStorage);
	sendTriggerCallbacks(triggerStorage);
}


void Physics::updateCollisionsSequential()
{
	collisionStorage.clear();
	triggerStorage.clear();

	for (int i = 0; i < rigidbodies.size(); i++)
	{
		auto rb1 = rigidbodies[i];
		auto col1 = rb1->_attachedCollider;
		if (col1 == nullptr) continue;

		for (int j = i + 1; j < rigidbodies.size(); j++)
		{
			auto rb2 = rigidbodies[j];
			auto col2 = rb2->_attachedCollider;
			if (col2 == nullptr) continue;

			auto collision = col1->getCollisionWith(col2);
			if (!collision.has_value()) continue;

			if (col1->_isTrigger || col2->_isTrigger)
				triggerStorage.push_back(collision.value());
			else
				collisionStorage.push_back(collision.value());
		}
	}
}

void Physics::updateCollisionsParallel()
{
	collisionStorage.clear();
	triggerStorage.clear();
	collisionStorage.reserve(rigidbodies.size() * rigidbodies.size() / 2);
	triggerStorage.reserve(rigidbodies.size() * rigidbodies.size() / 2);

	std::mutex mutexCol;
	std::mutex mutexTrig;
	auto interactionCount = rigidbodies.size() * rigidbodies.size() / 2 - rigidbodies.size() / 2;
	for (int ind = 0; ind < TASK_COUNT; ind++)
	{
		int start = ind * interactionCount / TASK_COUNT;
		int end = (ind + 1) * interactionCount / TASK_COUNT;

		pool.push_task([start, end, &mutexCol, &mutexTrig]
		{
			for (int i = start; i < end; i++)
			{
				auto size = rigidbodies.size();
				auto y = 0;
				int j = i;
				while (j >= size - y - 1 && j > 0)
				{
					j -= size - y - 1;
					y++;
				}
				auto x = y + j + 1;

				auto col1 = rigidbodies[y]->_attachedCollider;
				if (col1 == nullptr) continue;

				auto col2 = rigidbodies[x]->_attachedCollider;
				if (col2 == nullptr) continue;

				auto collision = col1->getCollisionWith(col2);
				if (!collision.has_value()) continue;

				if (col1->_isTrigger || col2->_isTrigger)
				{
					mutexTrig.lock();
					triggerStorage.push_back(collision.value());
					mutexTrig.unlock();
				}
				else
				{
					mutexCol.lock();
					collisionStorage.push_back(collision.value());
					mutexCol.unlock();
				}
			}
		});
	}
	pool.wait_for_tasks();
}

void Physics::sendCollisionCallbacks(const std::vector<Collision>& collisions)
{
	for (auto& collision : collisions)
	{
		auto col1 = collision.col1;
		auto col2 = collision.col2;

		bool currentlyColliding = std::ranges::find(col1->collidingWith, col2) != col1->collidingWith.end();
		if (!currentlyColliding)
		{
			col1->collisionEntered(col2);
			col2->collisionEntered(col1);
		}
		col1->collisionStayed(col2);
		col2->collisionStayed(col1);
	}
}
void Physics::sendTriggerCallbacks(const std::vector<Collision>& triggers)
{
	for (auto& trigger : triggers)
	{
		auto col1 = trigger.col1;
		auto col2 = trigger.col2;

		bool currentlyTriggering = std::ranges::find(col1->triggeringWith, col2) != col1->triggeringWith.end();
		if (!currentlyTriggering)
		{
			col1->triggerEntered(col2);
			col2->triggerEntered(col1);
		}
		col1->triggerStayed(col2);
		col2->triggerStayed(col1);
	}
}
