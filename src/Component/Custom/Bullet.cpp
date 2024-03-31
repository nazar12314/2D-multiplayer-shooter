#include "Bullet.h"

#include <iostream>

#include "Collider.h"
#include "MyTime.h"
#include "Object.h"
#include "PolygonCollider.h"
#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Assets.h"

Bullet::Bullet(Object* obj, float speed): Component(obj), _speed(speed)
{
	obj->addComponent<SpriteRenderer>(new Texture("sprites/square.png"), glm::vec2(0.2f, 0.2f), 0, Color::red);
	obj->addComponent<PolygonCollider>(glm::vec2(0.2f, 0.2f), true);
	rb = obj->addComponent<RigidBody>();
}

void Bullet::fixedUpdate()
{
	rb->moveTo(obj->pos() + obj->up() * _speed * Time::fixedDeltaTime);
}

void Bullet::onTriggerEnter(Collider* other)
{
	if (other->obj->tag() == "Wall")
	{
		std::cout << "hit" << std::endl;
		Object::destroy(obj);
	}
}
