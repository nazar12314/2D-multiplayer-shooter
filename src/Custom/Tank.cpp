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
#include "TextRenderer.h"

Tank::Tank(GameObject* obj, const std::string& name, const Color& mainColor, bool controlledByPlayer):
	Component(obj), _name(name), _mainColor(mainColor), _controlledByPlayer(controlledByPlayer) {}

void Tank::start()
{
	gameObject()->setTag("Tank");

	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	addComponent<SpriteRenderer>(tex, glm::vec2(1, 1.2f), _mainColor.darken(0.15f));
	_col = addComponent<BoxCollider>(glm::vec2(1, 1.2f));
	_rb = addComponent<Rigidbody>(4, 8);

	createGun();
	createParts();
	createParticles();
	createNameTag();

	_remoteController = addComponent<TankRemoteController>();
	if (_controlledByPlayer)
		_playerController = addComponent<TankPlayerController>(_remoteController);
}
void Tank::createGun()
{
	_gunPivot = GameObject::create("gunPivot", transform())->transform();
	_gunPivot->setLocalPos(glm::vec2(0, 0.1f));

	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	_gun = GameObject::create("gun", _gunPivot)->addComponent<SpriteRenderer>(tex, glm::vec2(0.7f, 0.2f), _mainColor.lighten(0.5f), 2);
	_gun->transform()->setLocalPos(glm::vec2(_gun->size().x / 2.0f, 0));
}
void Tank::createParts() const
{
	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	auto leftPart = GameObject::create("leftPart", transform())->transform();
	leftPart->addComponent<SpriteRenderer>(tex, glm::vec2(0.2f, 1.2f), _mainColor.darken(0.3f), 1);
	leftPart->setLocalPos(glm::vec2(-0.4f, 0));

	auto rightPart = GameObject::create("rightPart", transform())->transform();
	rightPart->addComponent<SpriteRenderer>(tex, glm::vec2(0.2f, 1.2f), _mainColor.darken(0.3f), 1);
	rightPart->setLocalPos(glm::vec2(0.4f, 0));
}
void Tank::createParticles() const
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
	particleSystem->color().set(_mainColor, Color::randomLight(), _mainColor.withAlpha(0.4f), _mainColor.withAlpha(0.4f));
	particleSystem->setShape(glm::vec2(0.4f, 0.0f));
}
void Tank::createNameTag()
{
	_nameText = GameObject::create("nameText", transform())->addComponent<TextRenderer>(_name, glm::vec2(10, 1.3f), _mainColor.saturate(0.5f).lighten(0.1f), 50);
	_nameText->transform()->setLocalPos(glm::vec2(0, 2.0f));
}

void Tank::update()
{
	if (!_controlledByPlayer) return;
	if (_shootTimer <= 0 && (Input::isKeyDown(SDLK_SPACE) || Input::isMouseButtonDown(SDL_BUTTON_LEFT)))
	{
		_shootTimer = 1.0f / _firerate;
		_remoteController->requestShoot();
	}
	else
		_shootTimer -= Time::deltaTime();
}
void Tank::lateUpdate()
{
	_nameText->transform()->setPos(transform()->pos() + glm::vec2(0, 2.0f));
	_nameText->transform()->setRot(0);
}

void Tank::shoot(bool silent)
{
	if (!silent)
		didShoot = true;

	float angle = glm::radians(_gun->transform()->rot());
	auto dir = glm::vec2(cos(angle), sin(angle));

	auto spawnPos = _gun->transform()->pos() + dir * _gun->size().x * 0.5f;
	auto bullet = GameObject::create("bullet", spawnPos, _gun->transform()->rot());
	bullet->addComponent<Bullet>(_col, 16, true);
}

Transform* Tank::gunPivot() const { return _gunPivot; }
Rigidbody* Tank::rb() const { return _rb; }
TankPlayerController* Tank::playerController() const { return _playerController; }
TankRemoteController* Tank::remoteController() const { return _remoteController; }
