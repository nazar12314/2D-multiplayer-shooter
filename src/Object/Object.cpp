#include "Object.h"

#include <iostream>

#include "Component.h"
#include "Texture.h"
#include "glm/gtx/transform.hpp"

// -- Global --
Object* Object::create(const std::string& name, glm::vec2 pos, float rot)
{
	auto obj = new Object(name, pos, rot);
	objectsToAddTemp.emplace_back(obj);

	return obj;
}
void Object::destroy(Object* obj)
{
	objects.erase(std::ranges::find(objects, obj));
	delete obj;
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
		if (!obj->enabled) continue;
		obj->update();
	}
}
void Object::lateUpdateAll()
{
	for (Object* obj : objects)
	{
		if (!obj->enabled) continue;
		obj->lateUpdate();
	}
}
void Object::fixedUpdateAll()
{
	for (Object* obj : objects)
	{
		if (!obj->enabled) continue;
		obj->fixedUpdate();
	}
}
void Object::destroyAll()
{
	for (Object* obj : objects)
		obj->destroy();
}

void Object::prepareAll()
{
	objects.insert(objects.end(), objectsToAddTemp.begin(), objectsToAddTemp.end());
	objectsToAddTemp.clear();

	for (Object* obj : objects)
		obj->transformChanged = false;
}
// -- Global --


Object::Object(const std::string& name, glm::vec2 pos, float rot) : Transform(pos, rot), name(name) {}
Object::~Object() {}

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
void Object::destroy() const
{
	for (Component* component : components)
		component->destroy();
}
