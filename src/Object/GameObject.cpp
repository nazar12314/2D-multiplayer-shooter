#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "Assets.h"
#include "glm/gtx/transform.hpp"

// -- Global --
GameObjectSPtr GameObject::create(const std::string& name, glm::vec2 pos, float rot)
{
	auto obj = create_internal(name, pos, rot);
	gameObjects.push_back(obj);

	return obj;
}
GameObjectSPtr GameObject::create(const std::string& name, Transform* parent)
{
	auto obj = create(name, {0, 0}, 0);
	obj->transform()->setParent(parent);
	obj->transform()->setLocalPos({0, 0});

	return obj;
}

void GameObject::destroyAll()
{
	for (int i = 0; i < gameObjects.size(); i++)
		destroy(gameObjects[i].get());
}
void GameObject::prepare()
{
	gameObjects.apply_changes();
	for (int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->_components.apply_changes();
}

GameObject::GameObject(const std::string& name, glm::vec2 pos, float rot) : _name(name), _transform(addComponent<Transform>(pos, rot)) { }

void GameObject::preDestroyInternal()
{
	if (_isDestroyed) return;
	for (const auto& comp : _components)
		_toDestroy.push_back(comp.get());
}
void GameObject::destroyImmediate()
{
	Object::destroyImmediate();
	gameObjects.erase_delayed_if([this](const GameObjectSPtr& obj) { return obj.get() == this; });
}

void GameObject::setActiveRecursively(bool enabled)
{
	throwIfDestroyed();
	_active = enabled;

	for (const auto& comp : _components)
		comp->setEnabled(enabled);

	for (const auto& child : _transform->children())
		child->gameObject()->setActiveRecursively(enabled);
}

std::string GameObject::name() const
{
	throwIfDestroyed();
	return _name;
}
std::string GameObject::tag() const
{
	throwIfDestroyed();
	return _tag;
}
bool GameObject::active() const
{
	throwIfDestroyed();
	return _active;
}
Transform* GameObject::transform() const
{
	throwIfDestroyed();
	return _transform;
}

void GameObject::setName(const std::string& name)
{
	throwIfDestroyed();
	_name = name;
}
void GameObject::setTag(const std::string& tag)
{
	throwIfDestroyed();
	_tag = tag;
}
void GameObject::setActive(bool enabled)
{
	throwIfDestroyed();
	setActiveRecursively(enabled);
}
