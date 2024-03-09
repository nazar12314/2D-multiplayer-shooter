// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <vector>

#include "Action.h"
#include "Transform.h"

class Component;
class Texture;

class Object : public Transform
{
public:
	// -- Global --
	inline static std::vector<Object*> objects {};

	inline static Action<Object*> onObjectAdded {};
	inline static Action<Object*> onObjectRemoved {};

	static void addObject(Object* obj);
	static void removeObject(Object* obj);

	static void startAll();
	static void updateAll();
	static void destroyAll();
	// -- Global --

private:
	std::vector<Component*> components {};

public:
	bool enabled;

	Object(glm::vec2 pos = {0, 0}, float rot = 0);
	virtual ~Object();

	template <typename T> T* addComponent();
	template <typename T> T* getComponent();
	template <typename T> void removeComponent();

	virtual void start() const;
	virtual void update() const;
	virtual void destroy() const;
};


template <typename T> T* Object::addComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
	T* component = new T(this);
	components.push_back(component);

	component->start();
	return component;
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

template <typename T> void Object::removeComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

	for (auto it = components.begin(); it != components.end(); ++it)
	{
		T* component = dynamic_cast<T*>(*it);
		if (component == nullptr) continue;
		component->destroy();

		components.erase(it);
		delete component;
	}

	std::erase_if(components, [](Component* c) { return dynamic_cast<T*>(c) != nullptr; });
}
