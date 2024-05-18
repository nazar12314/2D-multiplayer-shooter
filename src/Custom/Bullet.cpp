#include "Bullet.h"

#include "Collider.h"
#include "MyTime.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Assets.h"
#include "CircleCollider.h"
#include "Physics.h"
#include "PlayerManager.h"
#include "Tank.h"
#include "Transform.h"

Bullet::Bullet(GameObject* obj, Tank* shooter, Collider* colliderToIgnore, float speed, bool explode):
	Component(obj), _speed(speed), _explode(explode), _shooter(shooter), _colliderToIgnore(colliderToIgnore)
{
	auto tex = Assets::load<Sprite>("assets/sprites/circle.png");
	addComponent<SpriteRenderer>(tex, glm::vec2(0.2f, 0.2f), Color::RED);
	addComponent<CircleCollider>(0.1f, true);
	_rb = addComponent<Rigidbody>(true);
}

void Bullet::fixedUpdate()
{
	_rb->moveTo(transform()->pos() + transform()->right() * _speed * Time::fixedDeltaTime);
}

void Bullet::onTriggerEnter(Collider* other)
{
	if (other == _colliderToIgnore) return;

	std::basic_string tag = other->gameObject()->tag();
	if (tag != "Wall" && tag != "Tank") return;

	if (_explode)
		Physics::createImpact(transform()->pos(), 5.0f, 10.0f);

	if (tag == "Tank")
		other->getComponent<Tank>()->kill(_shooter);
	else if (tag == "Wall")
		other->getComponent<SpriteRenderer>()->setColor(_shooter->player()->color);

	destroy(gameObject());
}
