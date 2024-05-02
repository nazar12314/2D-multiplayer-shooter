#pragma once

#include "Object.h"
#include "GameObject.h"
#include "Concepts.h"

class Collider;
class Component;

class Component : public Object
{
	GameObject* _gameObject;
	bool _enabled = true;

	// -- General --

	// Called right after the component is created
	virtual void awake() {}

	// Called after the component is created and after awake
	virtual void start() {}

	// Called every frame
	virtual void update() {}

	// Called every frame after update
	virtual void lateUpdate() {}

	// Called every fixed frame
	virtual void fixedUpdate() {}

	// Called on component destruction
	virtual void onDestroy() {}

	// -- Collision --

	// Called when a collision starts
	virtual void onCollisionEnter(Collider* other) {}

	// Called when a collision stays, every frame
	virtual void onCollisionStay(Collider* other) {}

	// Called when a collision ends
	virtual void onCollisionExit(Collider* other) {}

	// Called when a trigger starts
	virtual void onTriggerEnter(Collider* other) {}

	// Called when a trigger stays, every frame
	virtual void onTriggerStay(Collider* other) {}

	// Called when a trigger ends
	virtual void onTriggerExit(Collider* other) {}

	// -- Mouse Events --

	// Called when the mouse enters the collider
	virtual void onMouseEnter() { }

	// Called when the mouse exits the collider
	virtual void onMouseExit() { }

	// Called on mouse click on the collider
	virtual void onMouseClick() { }

protected:
	Component(GameObject* gameObject);

	void destroyImmediate() override;

public:
	~Component() override {}

	GameObject* gameObject() const;
	Transform* transform() const;

	// Method forwarding to Object
	template <derived<Component> T, typename... Ts> T* addComponent(T* component);
	template <derived<Component> T, typename... Ts> T* addComponent(Ts&&... args);
	template <derived<Component> T> bool hasComponent() const;
	template <derived<Component> T> T* getComponent();
	template <derived<Component> T> bool tryGetComponent(T*& component) const;

	friend class GameObject;
	friend class Application;
	friend class Collider;
	friend class Physics;
	friend class Input;
};

template <derived<Component> T, typename ... Ts> T* Component::addComponent(T* component)
{
	return gameObject()->addComponent<T>(component);
}
template <derived<Component> T, typename... Ts> T* Component::addComponent(Ts&&... args)
{
	return gameObject()->addComponent<T>(std::forward<Ts>(args)...);
}
template <derived<Component> T> bool Component::hasComponent() const
{
	return gameObject()->hasComponent<T>();
}
template <derived<Component> T> T* Component::getComponent()
{
	return gameObject()->getComponent<T>();
}
template <derived<Component> T> bool Component::tryGetComponent(T*& component) const
{
	return gameObject()->tryGetComponent<T>(component);
}