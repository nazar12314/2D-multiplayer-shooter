#include "Tank.h"

#include "Bullet.h"
#include "Camera.h"
#include "Input.h"
#include "MyMath.h"
#include "MyTime.h"
#include "GameObject.h"
#include "PolygonCollider.h"
#include "Assets.h"
#include "Physics.h"
#include "SpriteRenderer.h"
#include "TankController.h"
#include "Rigidbody.h"
#include "TankEffects.h"
#include "Transform.h"
#include "glm/geometric.hpp"

Tank::Tank(GameObject* obj, bool explosiveBullets, bool explodeAtMousePosition): Component(obj), _explosiveBullets(explosiveBullets),
                                                                                 _explodeAtMousePosition(explodeAtMousePosition)
{
	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	addComponent<SpriteRenderer>(tex, glm::vec2(1, 1.2f), Color::randomLight());
	addComponent<BoxCollider>(glm::vec2(1, 1.2f));
	addComponent<Rigidbody>(4, 8);
	addComponent<TankController>();
	addComponent<TankEffects>();

	createGun();

	auto leftPart = GameObject::create("leftPart", transform())->transform();
	leftPart->addComponent<SpriteRenderer>(tex, glm::vec2(0.2f, 1.2f), getComponent<SpriteRenderer>()->color().darken(0.35f), 1);
	leftPart->setLocalPos(glm::vec2(-0.4f, 0));

	auto rightPart = GameObject::create("rightPart", transform())->transform();
	rightPart->addComponent<SpriteRenderer>(tex, glm::vec2(0.2f, 1.2f), getComponent<SpriteRenderer>()->color().darken(0.35f), 1);
	rightPart->setLocalPos(glm::vec2(0.4f, 0));
}
void Tank::createGun()
{
	gunPivot = GameObject::create("gunPivot", transform())->transform();
	gunPivot->setLocalPos(glm::vec2(0, 0.1f));

	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	gun = GameObject::create("gun", gunPivot)->addComponent<SpriteRenderer>(tex, glm::vec2(0.7f, 0.2f), getComponent<SpriteRenderer>()->color().lighten(0.5f), 2);
	gun->transform()->setLocalPos(glm::vec2(gun->size().x / 2.0f, 0));

	updateGunPosition();
}

void Tank::update()
{
	updateGunPosition();

	if (_shootCooldown <= 0 && (Input::isKeyDown(SDLK_SPACE) || Input::isMouseButtonDown(SDL_BUTTON_LEFT)))
	{
		_shootCooldown = 1.0f / _firerate;
		if (_explodeAtMousePosition)
		{
			auto mousePos = Input::mousePos();
			auto mouseWorldPos = Camera::getMain()->screenToWorldPoint(mousePos);
			Physics::createImpact(mouseWorldPos, 5.0f, 10.0f);
		}
		else
			shoot();
	}
	else
		_shootCooldown -= Time::deltaTime();
}

void Tank::updateGunPosition() const
{
	auto mousePos = Input::mousePos();
	auto mouseWorldPos = Camera::getMain()->screenToWorldPoint(mousePos);

	auto targetDir = normalize(mouseWorldPos - transform()->getPos());

	float rotateBy = _gunRotSpeed * Time::deltaTime();
	auto newRot = Math::rotateTowardsDir(gunPivot->transform()->getRot(), targetDir, rotateBy);
	if (!Math::nearlyEqual(newRot, gunPivot->transform()->getRot()))
		gunPivot->transform()->setRot(newRot);
}

void Tank::shoot() const
{
	float angle = glm::radians(gun->transform()->getRot());
	auto dir = glm::vec2(cos(angle), sin(angle));

	auto spawnPos = gun->transform()->getPos() + dir * gun->size().x * 0.5f;
	auto bullet = GameObject::create("bullet", spawnPos, gun->transform()->getRot());
	bullet->addComponent<Bullet>(16, _explosiveBullets);
}
