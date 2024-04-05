// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <string>

#include "Action.h"
#include "VectorDelayed.h"
#include "Concepts.h"
#include "glm/vec2.hpp"

class Transform;
class Collider;
class Component;
class Texture;

class Object final
{
public:
	// -- Global --
	inline static VectorDelayed<Object*> objects {};

	inline static Action<Component*> onComponentAddedGlobal;
	inline static Action<Component*> onComponentRemovedGlobal;

	static Object* create(const std::string& name = "New Object", glm::vec2 pos = {0, 0}, float rot = 0);
	static Object* create(const std::string& name, Transform* parent);
	static void destroy(Object* obj);

	static void destroyAll();

private:
	inline static std::vector<Object*> _toDelete {};

	static void prepareAll();
	template <typename... Ts> static void sendCallbackAll(void (Component::*func)(Ts...), Ts... args);
	// -- Global --

	std::string _name;
	std::string _tag;
	bool _enabled = true;

	VectorDelayed<Component*> _components {};
	Transform* _transform;

	Object(const std::string& name = "New Object", glm::vec2 pos = {0, 0}, float rot = 0);

	void prepare();
	template <typename... Ts> void sendCallback(void (Component::*func)(Ts...), Ts... args);

public:
	std::string name() const;
	std::string tag() const;
	bool enabled() const;

	Transform* transform() const { return _transform; }

	void setName(const std::string& name);
	void setTag(const std::string& tag);
	void setEnabled(bool enabled);

	template <derived<Component> T, typename... Ts> T* addComponent(Ts... args);
	template <derived<Component> T> void removeComponent();
	void removeComponent(Component* component);

	template <derived<Component> T> bool hasComponent() const;
	template <derived<Component> T> T* getComponent();
	template <derived<Component> T> bool tryGetComponent(T*& component) const;

	friend class Component;
	friend class Collider;
	friend class Input;
	friend class Application;
	friend class Physics;
};

template <typename... Ts> void Object::sendCallbackAll(void (Component::*func)(Ts...), Ts... args)
{
	for (Object* obj : objects)
		obj->sendCallback(func, args...);
}
template <typename... Ts> void Object::sendCallback(void (Component::*func)(Ts...), Ts... args)
{
	for (Component* component : _components)
		(component->*func)(args...);
}

template <derived<Component> T, typename... Ts> T* Object::addComponent(Ts... args)
{
	T* component = new T(this, args...);
	_components.push_back(component);

	onComponentAddedGlobal(component);
	component->start();
	return component;
}
template <derived<Component> T> bool Object::hasComponent() const
{
	for (Component* component : _components)
	{
		T* casted = dynamic_cast<T*>(component);
		if (casted == nullptr) continue;
		return true;
	}
	return false;
}
template <derived<Component> T> T* Object::getComponent()
{
	for (Component* component : _components)
	{
		T* casted = dynamic_cast<T*>(component);
		if (casted == nullptr) continue;
		return casted;
	}
	return nullptr;
}
template <derived<Component> T> bool Object::tryGetComponent(T*& component) const
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
template <derived<Component> T> void Object::removeComponent()
{
	for (auto it = _components.begin(); it != _components.end(); ++it)
	{
		T* component = dynamic_cast<T*>(*it);
		if (component == nullptr) continue;
		removeComponent(component);
	}
}
