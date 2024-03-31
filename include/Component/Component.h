#pragma once

class Collider;
class Object;

class Component
{
public:
	Object* obj;

	Component(Object* obj);
	virtual ~Component();

	// -- Default --
	virtual void start() {}
	virtual void update() {}
	virtual void lateUpdate() {}
	virtual void fixedUpdate() {}
	virtual void onDestroy() {}

	// -- Collision --
	virtual void onCollisionEnter(Collider* other) {}
	virtual void onCollisionStay(Collider* other) {}
	virtual void onCollisionExit(Collider* other) {}
};
