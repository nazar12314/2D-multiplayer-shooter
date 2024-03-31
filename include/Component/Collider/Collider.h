#pragma once

#include <vector>

#include "Component.h"

class Collider : public Component
{
	std::vector<Collider*> triggeringWith {};
	bool _isTrigger = false;

protected:
	bool recalculateOnTransformChange = true;

public:
	explicit Collider(Object* obj, bool isTrigger = false);
	~Collider() override = default;

	void start() override;

	bool isTrigger() const;
	void setIsTrigger(bool trigger);

	void lateUpdate() override;

	virtual void recalculate() {}
	virtual bool intersectsWith(Collider* other);

	void collisionEntered(Collider* other) const;
	void collisionStayed(Collider* other) const;
	void collisionExited(Collider* other) const;

	void triggerEntered(Collider* other);
	void triggerStayed(Collider* other) const;
	void triggerExited(Collider* other);

	friend class Physics;
	friend class RigidBody;
};
