#include "Tank.h"

#include "Bullet.h"
#include "Camera.h"
#include "Input.h"
#include "MyMath.h"
#include "MyTime.h"
#include "GameObject.h"
#include "PolygonCollider.h"
#include "Assets.h"
#include "ParticleSystem.h"
#include "Physics.h"
#include "SpriteRenderer.h"
#include "TankPlayerController.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "TankRemoteController.h"

Tank::Tank(GameObject* obj, bool controlledByPlayer): Component(obj), _controlledByPlayer(controlledByPlayer) {}

void Tank::start()
{
	gameObject()->setTag("Tank");

	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	addComponent<SpriteRenderer>(tex, glm::vec2(1, 1.2f), Color::randomLight().darken(0.15f));
	_col = addComponent<BoxCollider>(glm::vec2(1, 1.2f));
	addComponent<Rigidbody>(4, 8);

	createGun();
	createParts();
	createParticles();

	if (_controlledByPlayer)
		addComponent<TankPlayerController>();
	else
		addComponent<TankRemoteController>();
}
void Tank::createGun()
{
	_gunPivot = GameObject::create("gunPivot", transform())->transform();
	_gunPivot->setLocalPos(glm::vec2(0, 0.1f));

	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	_gun = GameObject::create("gun", _gunPivot)->addComponent<SpriteRenderer>(tex, glm::vec2(0.7f, 0.2f), getComponent<SpriteRenderer>()->color().lighten(0.5f), 2);
	_gun->transform()->setLocalPos(glm::vec2(_gun->size().x / 2.0f, 0));
}
void Tank::createParts()
{
	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	auto leftPart = GameObject::create("leftPart", transform())->transform();
	leftPart->addComponent<SpriteRenderer>(tex, glm::vec2(0.2f, 1.2f), getComponent<SpriteRenderer>()->color().darken(0.3f), 1);
	leftPart->setLocalPos(glm::vec2(-0.4f, 0));

	auto rightPart = GameObject::create("rightPart", transform())->transform();
	rightPart->addComponent<SpriteRenderer>(tex, glm::vec2(0.2f, 1.2f), getComponent<SpriteRenderer>()->color().darken(0.3f), 1);
	rightPart->setLocalPos(glm::vec2(0.4f, 0));
}
void Tank::createParticles()
{
	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	auto particleSystem = GameObject::create("particles", transform())->addComponent<ParticleSystem>(tex, Color::WHITE, -1);
	particleSystem->transform()->setLocalPos(glm::vec2(0, -0.5f));
	particleSystem->emissionRate().set(5);
	particleSystem->rateOverDistance().set(2.0f);
	particleSystem->lifetime().set(0.5f, 1);
	particleSystem->speed().set(0.4f, 1);
	particleSystem->rot().set(0, 360);
	particleSystem->scale().set(0.2f, 0.4f, 0, 0);
	particleSystem->color().set(getComponent<SpriteRenderer>()->color(), Color::randomLight(), Color::randomLight().withAlpha(0.4f), Color::randomLight().withAlpha(0.4f));
	particleSystem->setShape(glm::vec2(0.4f, 0.0f));
}

void Tank::update()
{
	if (!_controlledByPlayer) return;

	if (_shootTimer <= 0 && (Input::isKeyDown(SDLK_SPACE) || Input::isMouseButtonDown(SDL_BUTTON_LEFT)))
	{
		_shootTimer = 1.0f / _firerate;
		shoot();
	}
	else
		_shootTimer -= Time::deltaTime();
}

void Tank::shoot()
{
	didShoot = true;

	float angle = glm::radians(_gun->transform()->getRot());
	auto dir = glm::vec2(cos(angle), sin(angle));

	auto spawnPos = _gun->transform()->getPos() + dir * _gun->size().x * 0.5f;
	auto bullet = GameObject::create("bullet", spawnPos, _gun->transform()->getRot());
	bullet->addComponent<Bullet>(_col, 16, true);
}

Transform* Tank::gunPivot() const { return _gunPivot; }
