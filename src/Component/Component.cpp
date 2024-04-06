#include "Component.h"

Component::Component(GameObject* gameObject) : Object(), _gameObject(gameObject) { }
void Component::destroyImmediate()
{
	onDestroy();

	Object::destroyImmediate();

	GameObject::onComponentRemovedGlobal(this);
	_gameObject->_components.erase_delayed(this);
}
