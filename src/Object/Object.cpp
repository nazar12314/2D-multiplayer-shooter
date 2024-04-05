#include "Object.h"

#include "Component.h"
#include "Transform.h"
#include "Assets.h"
#include "glm/gtx/transform.hpp"

// -- Global --
Object* Object::create(const std::string& name, glm::vec2 pos, float rot)
{
	auto obj = new Object(name, pos, rot);
	objects.push_back_delayed(obj);

	return obj;
}
Object* Object::create(const std::string& name, Transform* parent)
{
	auto obj = create(name, {0, 0}, 0);
	obj->transform()->setParent(parent);
	obj->transform()->setLocalPos({0, 0});

	return obj;
}
void Object::destroy(Object* obj)
{
	sendCallbackAll(&Component::onDestroy);

	for (Component* comp : obj->_components)
		onComponentRemovedGlobal(comp);

	objects.erase_delayed(obj);
	_toDelete.push_back(obj);
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
	for (Object* obj : _toDelete)
		delete obj;
	_toDelete.clear();

	objects.apply_changes();
	for (Object* obj : objects)
		obj->prepare();
}
// -- Global --


Object::Object(const std::string& name, glm::vec2 pos, float rot) : _name(name)
{
	_transform = addComponent<Transform>(pos, rot);
}

std::string Object::name() const { return _name; }
std::string Object::tag() const { return _tag; }
bool Object::enabled() const { return _enabled; }

void Object::setName(const std::string& name) { _name = name; }
void Object::setTag(const std::string& tag) { _tag = tag; }
void Object::setEnabled(bool enabled) { _enabled = enabled; }

void Object::removeComponent(Component* component)
{
	component->onDestroy();

	_components.erase_delayed(component);
	onComponentRemovedGlobal(component);
}

void Object::prepare()
{
	_components.apply_changes();
}
