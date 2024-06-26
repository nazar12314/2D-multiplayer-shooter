#pragma once

#include <vector>

#include "Collider.h"
#include "Utils.h"
#include "VectorSafe.h"
#include "thread_pool_light/thread_pool_light.hpp"

class Gizmo;
class Solver;
class Rigidbody;

class Physics
{
	constexpr static int SUBSTEPS = 5; // Increase for precision, decrease for performance
	constexpr static int TASK_COUNT = 32;

	constexpr static bool MANUAL_UPDATE = false;
	constexpr static bool DISPLAY_GIZMOS_CONTACTS_DEBUG = false;
	constexpr static bool DISPLAY_GIZMOS_NORMALS_DEBUG = false;

	inline static VectorSafe<SPtr<Rigidbody>> rigidbodies;
	inline static VectorSafe<SPtr<Collider>> colliders;

	inline static float fixedUpdateTimer = 0;

	static void init();
	static void subscribeToEvents();

	// Physics loop
	static void physicsLoop();
	static void step(float dt, int substeps = SUBSTEPS);
	static void solveCollisions();

	// Collision detection
	inline static BS::thread_pool_light pool;

	inline static std::vector<Collision> collisionStorage;
	inline static std::vector<Collision> triggerStorage;

	static void updateCollisionsSequential();
	static void updateCollisionsParallel();

	static void updateContactPoints();

	// Callbacks
	static void sendCollisionCallbacks(const std::vector<Collision>& collisions);
	static void sendTriggerCallbacks(const std::vector<Collision>& triggers);

	inline static std::vector<Gizmo*> gizmos;
	static void displayGizmos_debug();
	static void clearGizmos_debug();

public:
	static SPtr<Collider> raycastAt(const glm::vec2& point);

	static void createImpact(const glm::vec2& point, float radius, float force);

	friend class Rigidbody;
	friend class Application;
};
