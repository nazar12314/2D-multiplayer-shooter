// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <stdexcept>
#include <string>

#include "Action.h"
#include "VectorDelayed.h"
#include "Concepts.h"
#include "Object.h"
#include "glm/vec2.hpp"

class Transform;
class Collider;
class Component;
class Sprite;

class GameObject final : public Object
{
	// -- Global --
public:
	inline static VectorDelayed<GameObject*> gameObjects {};

	inline static Action<Component*> onComponentAddedGlobal;
	inline static Action<Component*> onComponentRemovedGlobal;

	static GameObject* create(const std::string& name = "New Object", glm::vec2 pos = {0, 0}, float rot = 0);
	static GameObject* create(const std::string& name, Transform* parent);
	static void destroyAll();

private:
	static void prepare();
	template <typename... Ts> static void sendCallbackAll(void (Component::*func)(Ts...), Ts... args);
	// -- Global --


	// -- Local --
	std::string _name;
	std::string _tag;
	bool _active = true;

	VectorDelayed<Component*> _components {};
	Transform* _transform;

	GameObject(const std::string& name = "New Object", glm::vec2 pos = {0, 0}, float rot = 0);

	void destroyInternal() override;
	void destroyImmediate() override;

	template <typename... Ts> void sendCallback(void (Component::*func)(Ts...), Ts... args);

public:
	std::string name() const;
	std::string tag() const;
	bool active() const;

	Transform* transform() const;

	void setName(const std::string& name);
	void setTag(const std::string& tag);
	void setActive(bool enabled);

	template <derived<Component> T, typename... Ts> T* addComponent(Ts... args);
	template <derived<Component> T> void removeComponent();

	template <derived<Component> T> bool hasComponent() const;
	template <derived<Component> T> T* getComponent();
	template <derived<Component> T> bool tryGetComponent(T*& component) const;

	friend class Object;
	friend class Component;
	friend class Collider;
	friend class Input;
	friend class Application;
	friend class Physics;
};

template <typename... Ts> void GameObject::sendCallbackAll(void (Component::*func)(Ts...), Ts... args)
{
	for (GameObject* obj : gameObjects)
		obj->sendCallback(func, args...);
}
template <typename... Ts> void GameObject::sendCallback(void (Component::*func)(Ts...), Ts... args)
{
	throwIfDestroyed();
	for (Component* component : _components)
		(component->*func)(args...);
}

template <derived<Component> T, typename... Ts> T* GameObject::addComponent(Ts... args)
{
	T* component = new T(this, args...);
	_components.push_back(component);

	onComponentAddedGlobal(component);
	component->start();
	return component;
}
template <derived<Component> T> bool GameObject::hasComponent() const
{
	for (Component* component : _components)
	{
		T* casted = dynamic_cast<T*>(component);
		if (casted == nullptr) continue;
		return true;
	}
	return false;
}
template <derived<Component> T> T* GameObject::getComponent()
{
	for (Component* component : _components)
	{
		T* casted = dynamic_cast<T*>(component);
		if (casted == nullptr) continue;
		return casted;
	}
	return nullptr;
}
template <derived<Component> T> bool GameObject::tryGetComponent(T*& component) const
{
	for (Component* c : _components)
	{
		T* casted = dynamic_cast<T*>(c);
		if (casted == nullptr) continue;
		component = casted;
		return true;
	}
	return false;
}
template <derived<Component> T> void GameObject::removeComponent()
{
	for (auto it = _components.begin(); it != _components.end(); ++it)
	{
		T* component = dynamic_cast<T*>(*it);
		if (component == nullptr) continue;
		removeComponent(component);
	}
}
