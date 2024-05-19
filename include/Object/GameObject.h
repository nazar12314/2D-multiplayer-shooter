// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <memory>
#include <string>
#include <utility>

#include "Action.h"
#include "VectorSafe.h"
#include "Concepts.h"
#include "Object.h"
#include "glm/vec2.hpp"

class GameObject;
class Transform;
class Collider;
class Component;
class Sprite;

using GameObjectSPtr = std::shared_ptr<GameObject>;
using ComponentUPtr = std::unique_ptr<Component>;

class GameObject : public Object
{
	// -- Global --
public:
	inline static VectorSafe<GameObjectSPtr> gameObjects {};

	inline static Action<Component*> onComponentAddedGlobal;
	inline static Action<Component*> onComponentDestroyedGlobal;

	static GameObjectSPtr create(const std::string& name = "New Object", glm::vec2 pos = {0, 0}, float rot = 0);
	static GameObjectSPtr create(const std::string& name, Transform* parent);
	static void destroyAll();

private:
	static void prepare();
	template <typename... Ts> static void sendCallbackAll(void (Component::*func)(Ts...), Ts&&... args);
	// -- Global --


	// -- Local --
	std::string _name;
	std::string _tag;
	bool _active = true;

	VectorSafeRef<ComponentUPtr> _components {};
	Transform* _transform;

	GameObject(const std::string& name = "New Object", glm::vec2 pos = {0, 0}, float rot = 0);

	void preDestroyInternal() override;
	void destroyImmediate() override;

	template <typename... Ts> void sendCallback(void (Component::*func)(Ts...), Ts... args);

	void setActiveRecursively(bool enabled);

public:
	std::string name() const;
	std::string tag() const;
	bool active() const;

	Transform* transform() const;

	void setName(const std::string& name);
	void setTag(const std::string& tag);
	void setActive(bool enabled);

	template <derived<Component> T, typename... Ts> T* addComponent(T* component);
	template <derived<Component> T, typename... Ts> T* addComponent(Ts&&... args);
	template <derived<Component> T> bool hasComponent() const;
	template <derived<Component> T> T* getComponent();
	template <derived<Component> T> bool tryGetComponent(T*& outComponent) const;

	friend class Object;
	friend class Component;
	friend class Collider;
	friend class Input;
	friend class Application;
	friend class Physics;

private:
	// For shared_ptr creation
	template <typename... Arg> GameObjectSPtr static create_internal(Arg&&... args);
};

template <typename... Ts> void GameObject::sendCallbackAll(void (Component::*func)(Ts...), Ts&&... args)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (!gameObjects[i]->_active) continue;
		gameObjects[i]->sendCallback(func, std::forward<Ts>(args)...);
	}
}

#include "GameObject.ipp"