#pragma once

#include "Object.h"
#include "Concepts.h"

class Collider;

class Component
{
	Object* _obj;

	// -- General --
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

	// -- Mouse Events --
	virtual void onMouseEnter() { }
	virtual void onMouseExit() { }
	virtual void onMouseClick() { }

protected:
	Component(Object* obj);
	virtual ~Component();

public:
	Object* obj() const { return _obj; }
	Transform* transform() const { return _obj->_transform; }

	// Method forwarding to Object
	template <derived<Component> T, typename... Ts> T* addComponent(Ts... args);
	template <derived<Component> T> void removeComponent();
	template <derived<Component> T> bool hasComponent() const;
	template <derived<Component> T> T* getComponent();
	template <derived<Component> T> bool tryGetComponent(T*& component) const;

	friend class Object;
	friend class Application;
	friend class Collider;
	friend class Physics;
	friend class Input;
};

template <derived<Component> T, typename... Ts> T* Component::addComponent(Ts... args)
{
	return _obj->addComponent<T>(args...);
}
template <derived<Component> T> bool Component::hasComponent() const
{
	return _obj->hasComponent<T>();
}
template <derived<Component> T> T* Component::getComponent()
{
	return _obj->getComponent<T>();
}
template <derived<Component> T> bool Component::tryGetComponent(T*& component) const
{
	return _obj->tryGetComponent<T>(component);
}
template <derived<Component> T> void Component::removeComponent()
{
	_obj->removeComponent<T>();
}
