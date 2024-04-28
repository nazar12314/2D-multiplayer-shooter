#include "TankPlayerController.h"

#include "Camera.h"
#include "Input.h"
#include "GameObject.h"
#include "MyMath.h"
#include "MyTime.h"
#include "glm/vec2.hpp"
#include "Rigidbody.h"
#include "Transform.h"
#include "Tank.h"

TankPlayerController::TankPlayerController(GameObject* obj, float moveSpeed, float rotationSpeed): Component(obj), moveSpeed(moveSpeed), rotationSpeed(rotationSpeed) {}

void TankPlayerController::start()
{
	_tank = gameObject()->getComponent<Tank>();
	_rb = gameObject()->getComponent<Rigidbody>();

	updateGunPosition();
}

void TankPlayerController::update()
{
	updateGunPosition();
}
void TankPlayerController::updateGunPosition() const
{
	auto mousePos = Input::mousePos();
	auto mouseWorldPos = Camera::getMain()->screenToWorldPoint(mousePos);

	auto targetDir = normalize(mouseWorldPos - transform()->getPos());

	float rotateBy = _gunRotSpeed * Time::deltaTime();
	auto newRot = Math::rotateTowardsDir(_tank->_gunPivot->getRot(), targetDir, rotateBy);
	if (!Math::nearlyEqual(newRot, _tank->_gunPivot->getRot()))
		_tank->_gunPivot->setRot(newRot);
}

void TankPlayerController::fixedUpdate()
{
	float moveDir = 0;
	if (Input::isKeyDown(SDLK_w)) moveDir += 1;
	if (Input::isKeyDown(SDLK_s)) moveDir -= 1;

	float rotDir = 0;
	if (Input::isKeyDown(SDLK_a)) rotDir += 1;
	if (Input::isKeyDown(SDLK_d)) rotDir -= 1;

	float speedMult = Input::isKeyDown(SDLK_LSHIFT) ? 3 : 1;
	if (moveDir != 0)
	{
		_isMoving = true;
		_rb->addForce(transform()->up() * moveDir * moveSpeed * speedMult);
	}
	else _isMoving = false;
	if (rotDir != 0) _rb->addAngularForce(rotDir * rotationSpeed);
}

bool TankPlayerController::isMoving() const { return _isMoving; }
