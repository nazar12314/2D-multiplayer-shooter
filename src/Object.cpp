#include "Object.h"

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
	for (const auto obj : objects)
		obj->start();
}
void Object::updateAll()
{
	for (const auto obj : objects)
		obj->update();
}
void Object::destroyAll()
{
	for (const auto obj : objects)
		obj->onDestroy();
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
