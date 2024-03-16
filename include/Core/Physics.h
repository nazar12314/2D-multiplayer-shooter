#pragma once

#include <vector>

class RigidBody;

class Physics {
	static constexpr float EPSILON = 0.00001f;

	inline static std::vector<RigidBody*> rigidbodies;

	inline static float lastFixedUpdateTime = 0;

public:
	static void init();

	static void physicsLoop();
	static void fixedUpdate();
};
