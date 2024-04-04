#pragma once

class Collider;
class Object;

class Component
{
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

	virtual void onTriggerEnter(Collider* other) {}
	virtual void onTriggerStay(Collider* other) {}
	virtual void onTriggerExit(Collider* other) {}

public:
	Object* obj;

	Component(Object* obj);
	virtual ~Component();

	friend class Object;
};
