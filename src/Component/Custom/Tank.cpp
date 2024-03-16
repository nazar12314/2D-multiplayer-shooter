#include "Tank.h"

#include <iostream>

#include "Bullet.h"
#include "Camera.h"
#include "Input.h"
#include "Math.h"
#include "MyTime.h"
#include "Object.h"
#include "Texture.h"
#include "Sprite.h"
#include "glm/detail/func_geometric.inl"
#include "glm/detail/func_trigonometric.inl"

Tank::Tank(Object* obj, float gunRotSpeed, float firerate): Component(obj), _gunRotSpeed(gunRotSpeed), _firerate(firerate)
{
	auto gunTex = new Texture("sprites/square.png", Color::cyan());
	gun = Object::create("gun")->addComponent<Sprite>(gunTex, glm::vec2(0.2f, 0.7f));

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
	auto rot = MyMath::rotateTowards(gun->obj->rot(), targetRot, rotateBy);
	auto dir = glm::vec2(sin(glm::radians(rot)), cos(glm::radians(rot)));

	gun->obj->setPos(obj->pos() + glm::vec2(0, 0.1f) + dir * gun->size().y / 2.0f);
	gun->obj->setRot(rot);
}

void Tank::shoot() const
{
	float angle = glm::radians(gun->obj->rot());
	auto dir = glm::vec2(sin(angle), cos(angle));

	auto bullet = Object::create("bullet", obj->pos() + dir * gun->size().y * 0.85f, gun->obj->rot());
	bullet->addComponent<Sprite>(new Texture("sprites/square.png", Color::red()), glm::vec2(0.2f, 0.2f));
	bullet->addComponent<Bullet>();
}
