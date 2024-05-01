#include "TankPlayerController.h"

#include "Camera.h"
#include "Input.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Tank.h"
#include "TankRemoteController.h"
#include "Transform.h"
#include "glm/trigonometric.hpp"

TankPlayerController::TankPlayerController(GameObject* obj, TankRemoteController* remoteController): Component(obj), _remoteController(remoteController) {}

void TankPlayerController::start()
{
	_tank = gameObject()->getComponent<Tank>();
	_rb = gameObject()->getComponent<Rigidbody>();
}

void TankPlayerController::fixedUpdate()
{
	updateMovement();
	updateGunRotation();
}
void TankPlayerController::updateMovement() const
{
	auto moveDir = 0;
	if (Input::isKeyDown(SDLK_w)) moveDir += 1;
	if (Input::isKeyDown(SDLK_s)) moveDir -= 1;

	auto rotDir = 0;
	if (Input::isKeyDown(SDLK_a)) rotDir += 1;
	if (Input::isKeyDown(SDLK_d)) rotDir -= 1;

	auto speedMult = Input::isKeyDown(SDLK_LSHIFT) ? 3 : 1;
	if (moveDir != 0)
	{
		//_rb->addForce(transform()->up() * moveDir * _moveSpeed * speedMult);
		_remoteController->requestMove(moveDir * _moveSpeed * speedMult);
	}

	if (rotDir != 0)
	{
		//_rb->addAngularForce(rotDir * _rotationSpeed);
		_remoteController->requestRotate(rotDir * _rotationSpeed);
	}
}
void TankPlayerController::updateGunRotation() const
{
	auto mousePos = Input::mousePos();
	auto mouseWorldPos = Camera::getMain()->screenToWorldPoint(mousePos);

	auto targetDir = normalize(mouseWorldPos - transform()->pos());
	auto targetRot = glm::degrees(atan2(targetDir.y, targetDir.x));
	_remoteController->requestGunTargetRotation(targetRot);
}
