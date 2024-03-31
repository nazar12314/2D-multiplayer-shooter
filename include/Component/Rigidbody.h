#pragma once

#include "Component.h"
#include "Object.h"

class Collider;

class RigidBody : public Component
{
	glm::vec2 _velocity = {0, 0};
	float _angularVelocity = 0;

	Collider* attachedCollider = nullptr;

	RigidBody(Object* obj, float linearDrag = 0, float angularDrag = 0, float mass = 1);

public:
	float linearDrag, angularDrag;
	float mass;

	glm::vec2 velocity() const { return _velocity; }
	float angularVelocity() const { return _angularVelocity; }

	void setVelocity(glm::vec2 velocity);
	void addVelocity(glm::vec2 velocity);

	void setAngularVelocity(float angularVelocity);
	void addAngularVelocity(float angularVelocity);

	void moveTo(glm::vec2 pos) const;
	void rotateTo(float angle) const;

	Collider* getAttachedCollider();

	friend class Object;
	friend class Physics;
};
