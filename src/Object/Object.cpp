#include "Object.h"

#include <iostream>
#include "Component.h"
#include "Assets.h"
#include "glm/gtx/transform.hpp"

// -- Global --
Object* Object::create(const std::string& name, glm::vec2 pos, float rot)
{
	auto obj = new Object(name, pos, rot);
	objects.push_back_delayed(obj);

	return obj;
}
void Object::destroy(Object* obj)
{
	obj->onDestroy();

	objects.erase_delayed(obj);
}

void Object::startAll()
{
	for (Object* obj : objects)
		obj->start();
}
void Object::updateAll()
{
	for (Object* obj : objects)
	{
		if (!obj->_enabled) continue;
		obj->update();
	}
}
void Object::lateUpdateAll()
{
	for (Object* obj : objects)
	{
		if (!obj->_enabled) continue;
		obj->lateUpdate();
	}
}
void Object::fixedUpdateAll()
{
	for (Object* obj : objects)
	{
		if (!obj->_enabled) continue;
		obj->fixedUpdate();
	}
}
void Object::destroyAll()
{
	for (Object* obj : objects)
		obj->onDestroy();
}

void Object::prepareAll()
{
	objects.apply_changes();
	for (Object* obj : objects)
		obj->prepare();
}
// -- Global --


Object::Object(const std::string& name, glm::vec2 pos, float rot) : Transform(pos, rot), _name(name) {}
Object::~Object() {}

std::string Object::name() const { return _name; }
std::string Object::tag() const { return _tag; }
bool Object::enabled() const { return _enabled; }

void Object::setName(const std::string& name) { _name = name; }
void Object::setTag(const std::string& tag) { _tag = tag; }
void Object::setEnabled(bool enabled) { _enabled = enabled; }

void Object::removeComponent(Component* component)
{
	component->onDestroy();

	components.erase_delayed(component);
	onComponentRemovedGlobal(component);
}

void Object::prepare()
{
	transformChanged = false;
	components.apply_changes();
}

void Object::start() const
{
	for (Component* component : components)
		component->start();
}
void Object::update() const
{
	for (Component* component : components)
		component->update();
}
void Object::lateUpdate() const
{
	for (Component* component : components)
		component->lateUpdate();
}
void Object::fixedUpdate() const
{
	for (Component* component : components)
		component->fixedUpdate();
}
void Object::onDestroy()
{
	for (Component* component : components)
		removeComponent(component);
}

void Object::onCollisionEnter(Collider* other) const
{
	for (Component* component : components)
		component->onCollisionEnter(other);
}
void Object::onCollisionStay(Collider* other) const
{
	for (Component* component : components)
		component->onCollisionStay(other);
}
void Object::onCollisionExit(Collider* other) const
{
	for (Component* component : components)
		component->onCollisionExit(other);
}
void Object::onTriggerEnter(Collider* other) const
{
	for (Component* component : components)
		component->onTriggerEnter(other);
}
void Object::onTriggerStay(Collider* other) const
{
	for (Component* component : components)
		component->onTriggerStay(other);
}
void Object::onTriggerExit(Collider* other) const
{
	for (Component* component : components)
		component->onTriggerExit(other);
}
