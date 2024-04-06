#include "Object.h"

#include <stdexcept>

#include "GameObject.h"

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
	obj->destroyInternal();
	_toDestroy.push_back(obj);
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
