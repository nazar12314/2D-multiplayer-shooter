#pragma once

#include <vector>

#include "Collider.h"
#include "VectorDelayed.h"

class Solver;
class Rigidbody;

class Physics
{
	inline static VectorDelayed<Rigidbody*> rigidbodies;
	inline static std::vector<Solver*> solvers;

	inline static float lastFixedUpdateTime = 0;

	static void init();
	static void subscribeToEvents();

	static void physicsLoop();
	static void step(float dt);
	static void solveCollisions();

	static std::pair<std::vector<Collision>, std::vector<Collision>> findCollisions();

	static void sendCollisionCallbacks(const std::vector<Collision>& collisions);
	static void sendTriggerCallbacks(const std::vector<Collision>& triggers);

public:
	friend class Rigidbody;
	friend class Application;
};
