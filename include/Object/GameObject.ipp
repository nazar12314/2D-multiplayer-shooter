#include "GameObject.h"

#include "Component.h"

template <typename... Ts> void GameObject::sendCallback(void (Component::* func)(Ts...), Ts... args)
{
	throwIfDestroyed();
	for (const auto& component : _components)
	{
		if (!component->_enabled) continue;
		(component.get()->*func)(std::forward<Ts>(args)...);
	}
}

template <derived<Component> T, typename ... Ts> T* GameObject::addComponent(T* componentPtr)
{
	auto componentUPtr = std::unique_ptr<T>(componentPtr);
	_components.push_back(std::move(componentUPtr));

	componentPtr->awake();
	onComponentAddedGlobal(componentPtr);
	componentPtr->start();
	return componentPtr;
}
template <derived<Component> T, typename... Ts> T* GameObject::addComponent(Ts&&... args)
{
	return addComponent(new T(this, std::forward<Ts>(args)...));
}
template <derived<Component> T> bool GameObject::hasComponent() const
{
	for (const auto& component : _components)
	{
		auto casted = dynamic_cast<T*>(component.get());
		if (casted == nullptr) continue;
		return true;
	}
	return false;
}
template <derived<Component> T> T* GameObject::getComponent()
{
	for (const auto& component : _components)
	{
		auto casted = dynamic_cast<T*>(component.get());
		if (casted == nullptr) continue;
		return casted;
	}
	return nullptr;
}
template <derived<Component> T> bool GameObject::tryGetComponent(T*& outComponent) const
{
	for (const auto& component : _components)
	{
		auto casted = dynamic_cast<T*>(component.get());
		if (casted == nullptr) continue;
		outComponent = casted;
		return true;
	}
	return false;
}

template <typename... Ts> GameObjectSPtr GameObject::create_internal(Ts&&... args)
{
	struct EnableMakeShared : GameObject
	{
		EnableMakeShared(Ts&&... args) : GameObject(std::forward<Ts>(args)...) {}
	};
	return std::make_shared<EnableMakeShared>(std::forward<Ts>(args)...);
}
