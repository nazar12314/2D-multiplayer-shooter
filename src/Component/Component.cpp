#include "Component.h"

Component::Component(GameObject* gameObject) : _gameObject(gameObject) { }

void Component::destroyImmediate()
{
	onDestroy();
	// shared from this
	GameObject::onComponentDestroyedGlobal(shared_from_this());

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
void Component::setEnabled(bool enabled)
{
	if (enabled == _enabled) return;
	_enabled = enabled;

	if (_enabled)
		onEnable();
	else
		onDisable();
}
