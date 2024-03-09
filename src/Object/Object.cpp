#include "Object.h"

#include "Component.h"
#include "Texture.h"
#include "glm/gtx/transform.hpp"

// -- Global --
void Object::addObject(Object* obj)
{
	objects.emplace_back(obj);
	onObjectAdded(obj);
}
void Object::removeObject(Object* obj)
{
	objects.erase(std::ranges::find(objects, obj));
	onObjectRemoved(obj);
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
void Object::destroyAll()
{
	for (Object* obj : objects)
		obj->destroy();
}
// -- Global --


Object::Object(glm::vec2 pos, float rot) : Transform(pos, rot)
{
	addObject(this);
	enabled = true;
}
Object::~Object()
{
	removeObject(this);
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
void Object::destroy() const
{
	for (Component* component : components)
		component->destroy();
}
