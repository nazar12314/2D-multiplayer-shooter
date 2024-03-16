#pragma once
#include "Component.h"

class Collider : public Component
{
public:
	explicit Collider(Object* obj) : Component(obj) {}
	~Collider() override {}

	virtual bool collidesWith(const Collider* other) { return false; }
	virtual void onCollision(const Collider* other) {}
};
