#include "Bullet.h"

#include "MyTime.h"
#include "Object.h"
#include "Rigidbody.h"

Bullet::Bullet(Object* obj, float speed): Component(obj), _speed(speed)
{
	rb = obj->addComponent<RigidBody>();
}

void Bullet::fixedUpdate()
{
	rb->moveTo(obj->pos() + obj->up() * _speed * Time::fixedDeltaTime);
}
