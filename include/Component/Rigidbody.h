#pragma once

#include "Component.h"
#include "Object.h"

class Collider;

class Rigidbody : public Component
{
	bool _isStatic;

	float _mass = 1;
	float _invMass = 1;
	float _inertia = 0;
	float _invInertia = 0;
	float _restitution = 0;

	float _staticFriction = 0.6f;
	float _dynamicFriction = 0.4f;

	float _gravity = 0;

	// Linear
	float _linearDrag = 0;
	glm::vec2 _velocity = {0, 0};
	glm::vec2 _force = {0, 0};

	// Angular
	float _angularDrag = 0;
	float _angularVelocity = 0;
	float _angularForce = 0;

	Collider* _attachedCollider = nullptr;

	Rigidbody(Object* obj, bool isStatic = false);
	Rigidbody(Object* obj, float linearDrag, float angularDrag = 0);
	void initCollider();
	~Rigidbody() override;

	void step(float dt);
	void substep(float dt);

	void attachCollider(Collider* col);
	void resetCollider();

public:
	bool isStatic() const { return _isStatic; }
	float mass() const { return _mass; }
	float invMass() const { return _invMass; }
	float restitution() const { return _restitution; }
	float staticFriction() const { return _staticFriction; }
	float dynamicFriction() const { return _dynamicFriction; }
	float gravity() const { return _gravity; }
	float linearDrag() const { return _linearDrag; }
	float angularDrag() const { return _angularDrag; }
	glm::vec2 velocity() const { return _velocity; }
	float angularVelocity() const { return _angularVelocity; }

	void setIsStatic(bool isStatic);
	void setMass(float mass);
	void setRestitution(float restitution);
	void setStaticFriction(float staticFriction);
	void setDynamicFriction(float dynamicFriction);
	void setGravity(float gravity);
	void setLinearDrag(float linearDrag);
	void setAngularDrag(float angularDrag);
	void setVelocity(glm::vec2 velocity);
	void setAngularVelocity(float angularVelocity);

	void addForce(glm::vec2 force);
	void addAngularForce(float force);

	void moveTo(glm::vec2 pos) const;
	void rotateTo(float rot) const;

	void applyImpulse(glm::vec2 pos, glm::vec2 impulse);
	void applyImpact(glm::vec2 point, float radius, float force);

	friend class Object;
	friend class Physics;
	friend class Collider;
	friend class PositionSolver;
	friend class ImpulseSolver;
	friend class ImpactSolver;
};
