#include "Physics.h"

#include <execution>
#include <iostream>

#include "Collider.h"
#include "Gizmos.h"
#include "Input.h"
#include "MyTime.h"
#include "Rigidbody.h"
#include "Solver.h"
#include "Transform.h"

void Physics::init()
{
	subscribeToEvents();
}
void Physics::subscribeToEvents()
{
	GameObject::onComponentAddedGlobal += [](Component* comp)
	{
		if (auto rb = dynamic_cast<Rigidbody*>(comp))
			rigidbodies.push_back(rb);
		if (auto col = dynamic_cast<Collider*>(comp))
			colliders.push_back(col);
	};
	GameObject::onComponentDestroyedGlobal += [](Component* comp)
	{
		if (auto rb = dynamic_cast<Rigidbody*>(comp))
			rigidbodies.erase_delayed(rb);
		if (auto col = dynamic_cast<Collider*>(comp))
			colliders.erase_delayed(col);
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

	fixedUpdateTimer -= Time::deltaTime();
	while (fixedUpdateTimer <= 0)
	{
		Time::fixedTick();
		fixedUpdateTimer += Time::fixedDeltaTime;
		GameObject::sendCallbackAll(&Component::fixedUpdate);

		// !!! PHYSICS STEP !!!
		step(Time::fixedDeltaTime);
		// !!! PHYSICS STEP !!!

		if (Input::isKeyDown(SDLK_LCTRL)) // Speed up for testing
			for (int i = 0; i < 3; i++)
				step(Time::fixedDeltaTime);
	}
}

void Physics::step(float dt, int substeps)
{
	clearGizmos_debug();

	rigidbodies.apply_changes();
	colliders.apply_changes();
	for (int i = 0; i < rigidbodies.size(); i++)
		rigidbodies[i]->step(dt);

	for (int i = 0; i < substeps; i++)
	{
		rigidbodies.apply_changes();
		colliders.apply_changes();
		for (int j = 0; j < rigidbodies.size(); j++)
			rigidbodies[j]->substep(dt / substeps);

		solveCollisions();

		displayGizmos_debug();
	}
}

void Physics::displayGizmos_debug()
{
	if constexpr (!DISPLAY_GIZMOS_CONTACTS_DEBUG && !DISPLAY_GIZMOS_NORMALS_DEBUG) return;

	//updateCollisionsParallel();
	//updateContactPoints();

	if constexpr (DISPLAY_GIZMOS_CONTACTS_DEBUG)
		for (auto& col : collisionStorage)
			for (auto& point : col.contactPoints)
				gizmos.push_back(Gizmos::drawPoint(point, 0.03f, Color::RED, 999));

	if constexpr (DISPLAY_GIZMOS_NORMALS_DEBUG)
		for (auto& col : collisionStorage)
			for (auto& point : col.contactPoints)
				gizmos.push_back(Gizmos::drawVector(point, col.norm, glm::clamp(col.depth * 250, 0.0f, 5.0f), Color::GREEN, 999));
}
void Physics::clearGizmos_debug()
{
	for (auto& gizmo : gizmos)
		Gizmos::remove(gizmo);
	gizmos.clear();
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
	PositionSolver::solveCollisions(collisionStorage);
	updateContactPoints();
	ImpulseSolver::solveCollisions(collisionStorage);

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

void Physics::updateContactPoints()
{
	for (auto& collision : collisionStorage)
	{
		auto col1 = collision.col1;
		auto col2 = collision.col2;

		collision.contactPoints = col1->findContactPoints(col2);
	}
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

Collider* Physics::raycastAt(const glm::vec2& point)
{
	colliders.apply_changes();
	std::ranges::sort(colliders, [](const Collider* a, const Collider* b) { return a->transform()->getZ() < b->transform()->getZ(); });

	for (int i = colliders.size() - 1; i >= 0; i--)
	{
		auto col = colliders[i];
		if (col->isPointInside(point))
			return col;
	}
	return nullptr;
}

void Physics::createImpact(const glm::vec2& point, float radius, float force)
{
	for (int i = 0; i < rigidbodies.size(); i++)
		rigidbodies[i]->applyImpact(point, radius, force);
}
