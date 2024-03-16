#pragma once
#include "Component.h"

class Collider : public Component
{
public:
	explicit Collider(Object* obj) : Component(obj) {}
	~Collider() override = default;

	virtual bool collidesWith(Collider* other) { return false; }
	virtual void onCollision(const Collider* other) {}
};
