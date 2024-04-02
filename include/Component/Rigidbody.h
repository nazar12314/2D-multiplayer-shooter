#pragma once

#include "Component.h"
#include "Object.h"

class Collider;

class Rigidbody : public Component
{
	bool _isStatic = false;

	float _mass;
	float _invMass;
	float _inertia = 0;
	float _invInertia = 0;
	float _restitution = 0;

	// Linear
	float _linearDrag;
	glm::vec2 _velocity = {0, 0};
	glm::vec2 _force = {0, 0};

	// Angular
	float _angularDrag;
	float _angularVelocity = 0;
	float _angularForce = 0;

	Collider* _attachedCollider = nullptr;

	Rigidbody(Object* obj, float mass = 1, float linearDrag = 0, float angularDrag = 0);
	~Rigidbody() override;

	void step(float dt);

	void attachCollider(Collider* col);
	void resetCollider();

public:
	bool isStatic() const { return _isStatic; }
	float mass() const { return _mass; }
	float invMass() const { return _invMass; }
	float restitution() const { return _restitution; }
	float linearDrag() const { return _linearDrag; }
	float angularDrag() const { return _angularDrag; }
	glm::vec2 velocity() const { return _velocity; }
	float angularVelocity() const { return _angularVelocity; }

	void setIsStatic(bool isStatic);
	void setMass(float mass);
	void setRestitution(float restitution);
	void setLinearDrag(float linearDrag);
	void setAngularDrag(float angularDrag);
	void setVelocity(glm::vec2 velocity);
	void setAngularVelocity(float angularVelocity);

	void addForce(glm::vec2 force);
	void addAngularForce(float force);

	friend class Object;
	friend class Physics;
	friend class Collider;
	friend class PositionSolver;
	friend class ImpulseSolver;
};
