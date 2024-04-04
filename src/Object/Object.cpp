#include "Object.h"

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
	sendCallbackAll(&Component::onDestroy);

	objects.erase_delayed(obj);
	delete obj;
}
void Object::destroyAll()
{
	objects.apply_changes();
	for (Object* obj : objects)
		destroy(obj);
	objects.apply_changes();
}

void Object::prepareAll()
{
	objects.apply_changes();
	for (Object* obj : objects)
		obj->prepare();
}
// -- Global --


Object::Object(const std::string& name, glm::vec2 pos, float rot) : Transform(pos, rot), _name(name) {}

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
	components.apply_changes();
}
