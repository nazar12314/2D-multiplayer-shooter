#pragma once

#include <vector>

#include "Collider.h"
#include "VectorDelayed.h"
#include "thread_pool_light/thread_pool_light.hpp"

class Solver;
class Rigidbody;

class Physics
{
	constexpr static int SUBSTEPS = 4;
	constexpr static bool DISPLAY_CONTACT_POINTS = true;
	constexpr static int TASK_COUNT = 32;

	inline static VectorDelayed<Rigidbody*> rigidbodies;
	inline static std::vector<Solver*> solvers;

	inline static float fixedUpdateTimer = 0;

	static void init();
	static void subscribeToEvents();

	// Physics loop
	static void physicsLoop();
	static void step(float dt);
	static void solveCollisions();

	// Collision detection
	inline static BS::thread_pool_light pool;

	inline static std::vector<Collision> collisionStorage;
	inline static std::vector<Collision> triggerStorage;

	static void updateCollisionsSequential();
	static void updateCollisionsParallel();

	// Callbacks
	static void sendCollisionCallbacks(const std::vector<Collision>& collisions);
	static void sendTriggerCallbacks(const std::vector<Collision>& triggers);

	static void displayContactPoints_debug();

public:
	friend class Rigidbody;
	friend class Application;
};
