#pragma once

#include "Component.h"
#include <vector>

class Collider : public Component
{
	std::vector<Collider*> collidingWith {};

public:
	explicit Collider(Object* obj);
	~Collider() override = default;

	virtual bool collidesWith(Collider* other) { return false; }

	void collisionEntered(Collider* other);
	void collisionStayed(Collider* other) const;
	void collisionExited(Collider* other);

	friend class Physics;
};
