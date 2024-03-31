// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <string>
#include <vector>

#include "Action.h"
#include "Transform.h"
#include "VectorDelayed.h"
#include "glm/vec2.hpp"

class Collider;
class Component;
class Texture;

class Object : public Transform
{
public:
	// -- Global --
	inline static VectorDelayed<Object*> objects {};

	inline static Action<Component*> onComponentAddedGlobal;
	inline static Action<Component*> onComponentRemovedGlobal;

	static Object* create(const std::string& name = "New Object", glm::vec2 pos = {0, 0}, float rot = 0);
	static void destroy(Object* obj);

	static void startAll();
	static void updateAll();
	static void lateUpdateAll();
	static void fixedUpdateAll();
	static void destroyAll();

	static void prepareAll();
	// -- Global --

private:
	std::string _name;
	std::string _tag;
	bool _enabled = true;

	VectorDelayed<Component*> components {};

	Object(const std::string& name = "New Object", glm::vec2 pos = {0, 0}, float rot = 0);
	virtual ~Object();

public:
	std::string name() const;
	std::string tag() const;
	bool enabled() const;

	void setName(const std::string& name);
	void setTag(const std::string& tag);
	void setEnabled(bool enabled);

	template <typename T, typename... Ts> T* addComponent(Ts... args);
	template <typename T> void removeComponent();
	void removeComponent(Component* component);

	template <typename T> bool hasComponent() const;
	template <typename T> T* getComponent();
	template <typename T> bool tryGetComponent(T*& component) const;

protected:
	// -- System --
	void prepare();

	// -- Default --
	virtual void start() const;
	virtual void update() const;
	virtual void lateUpdate() const;
	virtual void fixedUpdate() const;
	virtual void onDestroy();

	// -- Collision --
	virtual void onCollisionEnter(Collider* other) const;
	virtual void onCollisionStay(Collider* other) const;
	virtual void onCollisionExit(Collider* other) const;

	friend class Collider;
};

template <typename T, typename... Ts> T* Object::addComponent(Ts... args)
{
	static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
	T* component = new T(this, args...);
	components.push_back(component);

	onComponentAddedGlobal(component);
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
		removeComponent(component);
	}
}
