#include "Tank.h"

#include <iostream>

#include "Bullet.h"
#include "Camera.h"
#include "Input.h"
#include "Math.h"
#include "MyTime.h"
#include "Object.h"
#include "PolygonCollider.h"
#include "Assets.h"
#include "SpriteRenderer.h"
#include "TankController.h"
#include "Rigidbody.h"

Tank::Tank(Object* obj, float gunRotSpeed, float firerate): Component(obj), _gunRotSpeed(gunRotSpeed), _firerate(firerate)
{
	obj->addComponent<SpriteRenderer>(new Texture("sprites/square.png"), glm::vec2(1, 1.2f), 0, Color::hot_pink);
	obj->addComponent<PolygonCollider>(glm::vec2(1, 1.2f));
	obj->addComponent<RigidBody>(10, 25);
	obj->addComponent<TankController>();

	createGun();
}
void Tank::createGun()
{
	auto gunTex = new Texture("sprites/square.png");
	gun = Object::create("gun")->addComponent<SpriteRenderer>(gunTex, glm::vec2(0.2f, 0.7f), 1, Color::cyan);

	updateGunPosition();
}


void Tank::update()
{
	updateGunPosition();

	if (shootCooldown <= 0 && (Input::isKeyDown(SDLK_SPACE) || Input::isMouseButtonDown(SDL_BUTTON_LEFT)))
	{
		shootCooldown = 1.0f / _firerate;
		shoot();
	}
	else
		shootCooldown -= Time::deltaTime;
}

void Tank::updateGunPosition() const
{
	auto mousePos = Input::mousePos;
	auto mouseWorldPos = Camera::getMain()->screenToWorldPos(mousePos);

	auto targetDir = normalize(mouseWorldPos - obj->pos());
	float targetRot = glm::degrees(atan2(targetDir.x, targetDir.y));

	float rotateBy = _gunRotSpeed * Time::deltaTime;
	auto newRot = Math::rotateTowards(gun->obj->rot(), targetRot, rotateBy);
	if (newRot != gun->obj->rot()) gun->obj->setRot(newRot);

	auto dir = glm::vec2(sin(glm::radians(newRot)), cos(glm::radians(newRot)));
	auto newPos = obj->pos() + glm::vec2(0, 0.1f) + dir * gun->size().y / 2.0f;
	if (newPos != gun->obj->pos()) gun->obj->setPos(newPos);
}

void Tank::shoot() const
{
	float angle = glm::radians(gun->obj->rot());
	auto dir = glm::vec2(sin(angle), cos(angle));

	auto spawnPos = obj->pos() + dir * gun->size().y * 0.85f;
	auto bullet = Object::create("bullet", spawnPos, gun->obj->rot());
	bullet->addComponent<Bullet>();
}
