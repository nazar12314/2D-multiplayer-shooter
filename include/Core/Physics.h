#pragma once

#include <vector>

#include "Collider.h"
#include "VectorDelayed.h"

class RigidBody;

class Physics
{
	static constexpr float EPSILON = 0.00001f;

	inline static VectorDelayed<RigidBody*> rigidbodies;

	inline static float lastFixedUpdateTime = 0;

	static void init();

	static void physicsLoop();
	static void updatePhysics();
	static void updateTriggers();
	static void updateTriggering(Collider* col1, Collider* col2, bool intersecting);

public:
	static std::vector<RigidBody*> getCollisions(const RigidBody* rb);

	friend class RigidBody;
	friend class Application;
};
