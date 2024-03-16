// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <string>
#include <vector>

#include "Action.h"
#include "Transform.h"
#include "glm/vec2.hpp"

class Component;
class Texture;

class Object : public Transform
{
public:
	// -- Global --
	inline static std::vector<Object*> objects {};

	inline static Action<Component*> onComponentAdded;
	inline static Action<Component*> onComponentRemoved;

	static Object* create(const std::string& name, glm::vec2 pos = {0, 0}, float rot = 0);
	static void destroy(Object* obj);

	static void startAll();
	static void updateAll();
	static void destroyAll();
	// -- Global --

private:
	std::vector<Component*> components {};

	Object(const std::string& name = "New Object", glm::vec2 pos = {0, 0}, float rot = 0);
	virtual ~Object();

public:
	std::string name;
	bool enabled = true;

	template <typename T, typename... Ts> T* addComponent(Ts... args);
	template <typename T> void removeComponent();

	template <typename T> bool hasComponent() const;
	template <typename T> T* getComponent();
	template <typename T> bool tryGetComponent(T*& component) const;

	virtual void start() const;
	virtual void update() const;
	virtual void destroy() const;
};

template <typename T, typename... Ts> T* Object::addComponent(Ts... args)
{
	static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
	T* component = new T(this, args...);
	components.push_back(component);

	onComponentAdded(component);
	component->start();
	return component;
}
template <typename T> bool Object::hasComponent() const
{
	static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
	for (Component* component : components)
	{
		T* casted = dynamic_cast<T*>(component);
		if (casted == nullptr) continue;
		return true;
	}
	return false;
}
template <typename T> T* Object::getComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
	for (Component* component : components)
	{
		T* casted = dynamic_cast<T*>(component);
		if (casted == nullptr) continue;
		return casted;
	}
	return nullptr;
}
template <typename T> bool Object::tryGetComponent(T*& component) const
{
	static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
	for (Component* c : components)
	{
		T* casted = dynamic_cast<T*>(c);
		if (casted == nullptr) continue;
		component = casted;
		return true;
	}
	return false;
}
template <typename T> void Object::removeComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

	for (auto it = components.begin(); it != components.end(); ++it)
	{
		T* component = dynamic_cast<T*>(*it);
		if (component == nullptr) continue;
		component->destroy();

		onComponentRemoved(component);
		components.erase(it);
		delete component;
	}

	std::erase_if(components, [](Component* c) { return dynamic_cast<T*>(c) != nullptr; });
}
