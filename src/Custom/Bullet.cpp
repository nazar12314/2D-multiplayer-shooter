#include "Bullet.h"

#include "Collider.h"
#include "MyTime.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Assets.h"
#include "CircleCollider.h"
#include "Physics.h"
#include "Transform.h"

Bullet::Bullet(GameObject* obj, float speed, bool explode): Component(obj), _speed(speed), _explode(explode)
{
	auto tex = Assets::load<Sprite>("assets/sprites/circle.png");
	addComponent<SpriteRenderer>(tex, glm::vec2(0.2f, 0.2f), Color::RED);
	addComponent<CircleCollider>(0.1f, true);
	rb = addComponent<Rigidbody>(true);
}

void Bullet::fixedUpdate()
{
	_immuneTimer -= Time::fixedDeltaTime;
	rb->moveTo(transform()->getPos() + transform()->right() * _speed * Time::fixedDeltaTime);
}

void Bullet::onTriggerEnter(Collider* other)
{
	if (_immuneTimer > 0) return;

	std::basic_string tag = other->gameObject()->tag();
	if (tag != "Wall" && tag != "Tank") return;

	if (_explode)
		Physics::createImpact(transform()->getPos(), 5.0f, 10.0f);
	destroy(gameObject());
}
