#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "Assets.h"
#include "glm/gtx/transform.hpp"

// -- Global --
GameObject* GameObject::create(const std::string& name, glm::vec2 pos, float rot)
{
	auto obj = new GameObject(name, pos, rot);
	gameObjects.push_back_delayed(obj);

	return obj;
}
GameObject* GameObject::create(const std::string& name, Transform* parent)
{
	auto obj = create(name, {0, 0}, 0);
	obj->transform()->setParent(parent);
	obj->transform()->setLocalPos({0, 0});

	return obj;
}

void GameObject::destroyAll()
{
	for (GameObject* obj : gameObjects)
		destroy(obj);
}
void GameObject::prepare()
{
	gameObjects.apply_changes();
	for (GameObject* obj : gameObjects)
		obj->_components.apply_changes();
}

GameObject::GameObject(const std::string& name, glm::vec2 pos, float rot) : Object(), _name(name)
{
	_transform = addComponent<Transform>(pos, rot);
}
void GameObject::destroyInternal()
{
	if (_isDestroyed) return;
	for (Component* comp : _components)
		destroy(comp);
}
void GameObject::destroyImmediate()
{
	Object::destroyImmediate();
	gameObjects.erase_delayed(this);
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
	_active = enabled;
}
