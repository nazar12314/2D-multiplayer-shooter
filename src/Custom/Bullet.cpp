#include "Bullet.h"

#include "Collider.h"
#include "MyTime.h"
#include "Object.h"
#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Assets.h"
#include "CircleCollider.h"
#include "Physics.h"
#include "Transform.h"

Bullet::Bullet(Object* obj, float speed, bool explode): Component(obj), _speed(speed), _explode(explode)
{
	auto tex = Assets::load<Texture>("sprites/circle.png");
	addComponent<SpriteRenderer>(tex, glm::vec2(0.2f, 0.2f), 0, Color::red);
	addComponent<CircleCollider>(0.1f, true);
	rb = addComponent<Rigidbody>(true);
}

void Bullet::fixedUpdate()
{
	rb->moveTo(transform()->getPos() + transform()->right() * _speed * Time::fixedDeltaTime);
}

void Bullet::onTriggerEnter(Collider* other)
{
	if (other->obj()->tag() != "Wall") return;

	if (_explode)
		Physics::createImpact(transform()->getPos(), 5.0f, 10.0f);
	Object::destroy(obj());
}
