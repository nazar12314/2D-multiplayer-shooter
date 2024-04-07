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
#include "Transform.h"
#include "glm/geometric.hpp"
#include "glm/detail/func_trigonometric.inl"

Tank::Tank(GameObject* obj, bool explosiveBullets, bool explodeAtMousePosition): Component(obj), _explosiveBullets(explosiveBullets),
                                                                                 _explodeAtMousePosition(explodeAtMousePosition)
{
	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	addComponent<SpriteRenderer>(tex, glm::vec2(1, 1.2f), Color::randomLight());
	addComponent<PolygonCollider>(glm::vec2(1, 1.2f));
	addComponent<Rigidbody>(10, 15);
	addComponent<TankController>();

	createGun();
}
void Tank::createGun()
{
	auto tex = Assets::load<Sprite>("assets/sprites/square.png");
	gun = GameObject::create("gun")->addComponent<SpriteRenderer>(tex, glm::vec2(0.7f, 0.2f), Color::randomLight(), 1);
	gunPivot = GameObject::create("gunPivot", transform())->transform();
	gunPivot->setLocalPos(glm::vec2(0, 0.1f));
	gun->transform()->setParent(gunPivot);
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
			auto mousePos = Input::mousePos;
			auto mouseWorldPos = Camera::getMain()->screenToWorldPoint(mousePos);
			Physics::createImpact(mouseWorldPos, 5.0f, 10.0f);
		}
		else
			shoot();
	}
	else
		_shootCooldown -= Time::deltaTime;
}

void Tank::updateGunPosition() const
{
	auto mousePos = Input::mousePos;
	auto mouseWorldPos = Camera::getMain()->screenToWorldPoint(mousePos);

	auto targetDir = normalize(mouseWorldPos - transform()->getPos());

	float rotateBy = _gunRotSpeed * Time::deltaTime;
	auto newRot = Math::rotateTowardsDir(gunPivot->transform()->getRot(), targetDir, rotateBy);
	if (!Math::nearlyEqual(newRot, gunPivot->transform()->getRot()))
		gunPivot->transform()->setRot(newRot);
}

void Tank::shoot() const
{
	float angle = glm::radians(gun->transform()->getRot());
	auto dir = glm::vec2(cos(angle), sin(angle));

	auto spawnPos = transform()->getPos() + dir * gun->size().x * 0.85f;
	auto bullet = GameObject::create("bullet", spawnPos, gun->transform()->getRot());
	bullet->addComponent<Bullet>(16, _explosiveBullets);
}
