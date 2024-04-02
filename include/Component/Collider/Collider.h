#pragma once

#include <vector>

#include "Collision.h"
#include "Component.h"

class CircleCollider;
class PolygonCollider;
class Rigidbody;

class Collider : public Component
{
	std::vector<Collider*> collidingWith {};
	std::vector<Collider*> triggeringWith {};
	bool _isTrigger = false;
	Rigidbody* _rb = nullptr;

	virtual Collision getCollisionWith(Collider* other) = 0;
	virtual Collision getCollisionWith(PolygonCollider* other) = 0;
	virtual Collision getCollisionWith(CircleCollider* other) = 0;

	void collisionEntered(Collider* other);
	void collisionStayed(Collider* other) const;
	void collisionExited(Collider* other);

	void triggerEntered(Collider* other);
	void triggerStayed(Collider* other) const;
	void triggerExited(Collider* other);

	virtual float calculateInertia(float mass) const = 0;

public:
	explicit Collider(Object* obj, bool isTrigger = false);
	~Collider() override;

	bool isTrigger() const;
	void setIsTrigger(bool trigger);

	friend class Physics;
	friend class Rigidbody;
	friend class PolygonCollider;
	friend class CircleCollider;
	friend class ImpulseSolver;
	friend class PositionSolver;
};
