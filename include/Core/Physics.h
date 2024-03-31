#pragma once

#include <vector>

class RigidBody;

class Physics
{
	static constexpr float EPSILON = 0.00001f;

	inline static std::vector<RigidBody*> rigidbodies;
	inline static std::vector<RigidBody*> rigidbodiesToAdd;
	inline static std::vector<RigidBody*> rigidbodiesToRemove;

	inline static float lastFixedUpdateTime = 0;

public:
	static void init();

	static void physicsLoop();
	static void performFixedUpdate();
	static void updateCollisions();
	static void addOrRemoveRequestedRigidbodies();
};
