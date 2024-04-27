#include "Object.h"

#include <stdexcept>

#include "DOTween.h"
#include "GameObject.h"
#include "Transform.h"

void Object::prepare()
{
	for (Object* obj : _toDestroy)
		obj->destroyImmediate();
	_toDestroy.clear();

	GameObject::prepare();
}
void Object::destroy(Object* obj)
{
	if (obj->_isDestroyed) return;
	if (dynamic_cast<Transform*>(obj))
		throw std::runtime_error("Cannot destroy Transform component, you should destroy GameObject itself.");

	obj->preDestroyInternal();
	_toDestroy.push_back(obj);
}
void Object::destroy(const GameObjectSPtr& obj)
{
	destroy(obj.get());
}

void Object::destroyImmediate()
{
	_isDestroyed = true;
}
void Object::throwIfDestroyed() const
{
	if (_isDestroyed)
		throw std::runtime_error("Trying to access destroyed object");
}

bool Object::isDestroyed() const { return _isDestroyed; }

void Object::doKill() const
{
	for (Tween* tween : DOVirtual::_tweens)
		if (tween->_target == this)
			tween->kill();
}
