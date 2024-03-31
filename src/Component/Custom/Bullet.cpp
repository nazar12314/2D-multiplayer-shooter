#include "Bullet.h"

#include <iostream>

#include "Collider.h"
#include "MyTime.h"
#include "Object.h"
#include "PolygonCollider.h"
#include "Rigidbody.h"
#include "Sprite.h"
#include "Texture.h"

Bullet::Bullet(Object* obj, float speed): Component(obj), _speed(speed)
{
	obj->addComponent<Sprite>(new Texture("sprites/square.png", Color::red()), glm::vec2(0.2f, 0.2f));
	obj->addComponent<PolygonCollider>(glm::vec2(0.2f, 0.2f));
	rb = obj->addComponent<RigidBody>();
}

void Bullet::fixedUpdate()
{
	rb->moveTo(obj->pos() + obj->up() * _speed * Time::fixedDeltaTime);
}

void Bullet::onCollisionEnter(Collider* other)
{
	if (other->obj->tag() == "Wall")
	{
		std::cout << "hit" << std::endl;
		Object::destroy(obj);
	}
}
