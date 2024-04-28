#include "Component.h"

Component::Component(GameObject* gameObject) : _gameObject(gameObject) { }

void Component::destroyImmediate()
{
	onDestroy();
	GameObject::onComponentDestroyedGlobal(this);

	Object::destroyImmediate();
	// TODO: Fix this
	//_gameObject->_components.erase_delayed_if([this](const ComponentUPtr& comp) { return comp.get() == this; });
}

GameObject* Component::gameObject() const
{
	throwIfDestroyed();
	return _gameObject;
}
Transform* Component::transform() const
{
	return gameObject()->transform();
}
