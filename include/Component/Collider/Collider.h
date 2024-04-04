#pragma once

#include <optional>
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

	explicit Collider(Object* obj, bool isTrigger = false);
	~Collider() override;

	void start() override;
	virtual void recalculate() {}

	virtual std::optional<Collision> getCollisionWith(Collider* other) = 0;
	virtual std::optional<Collision> getCollisionWith(PolygonCollider* other) = 0;
	virtual std::optional<Collision> getCollisionWith(CircleCollider* other) = 0;

	virtual std::vector<glm::vec2> findContactPoints(const Collider* other) const = 0;
	virtual std::vector<glm::vec2> findContactPoints(const PolygonCollider* other) const = 0;
	virtual std::vector<glm::vec2> findContactPoints(const CircleCollider* other) const = 0;

	virtual bool isPointInside(const glm::vec2& point) const = 0;

	void collisionEntered(Collider* other);
	void collisionStayed(Collider* other) const;
	void collisionExited(Collider* other);

	void triggerEntered(Collider* other);
	void triggerStayed(Collider* other) const;
	void triggerExited(Collider* other);

	virtual float calculateMass() const = 0;
	virtual float calculateInertia(float mass) const = 0;
public:
	bool isTrigger() const;
	void setIsTrigger(bool trigger);

	friend class Object;
	friend class Physics;
	friend class Rigidbody;
	friend class PolygonCollider;
	friend class CircleCollider;
	friend class ImpulseSolver;
	friend class PositionSolver;
};
