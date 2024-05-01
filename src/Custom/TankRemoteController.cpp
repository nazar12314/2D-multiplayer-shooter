#include "TankRemoteController.h"

#include "Camera.h"
#include "MyMath.h"
#include "MyTime.h"
#include "Rigidbody.h"
#include "Tank.h"
#include "Transform.h"

TankRemoteController::TankRemoteController(GameObject* obj): Component(obj) {}

void TankRemoteController::start()
{
	_rb = gameObject()->getComponent<Rigidbody>();
	_tank = gameObject()->getComponent<Tank>();
}

void TankRemoteController::moveTo(const glm::vec2& pos) const
{
	_rb->moveTo(pos);
}
void TankRemoteController::rotateTo(float rot) const
{
	_rb->rotateTo(rot);
}
void TankRemoteController::rotateGunTo(float rot) const
{
	_tank->_gunPivot->setRot(rot);
}
void TankRemoteController::shoot(bool shoot) const
{
	if (shoot) _tank->shoot(true);
}

void TankRemoteController::applyMovement(float movement) const
{
	_rb->addForce(transform()->up() * movement);
}
void TankRemoteController::applyRotation(float rotation) const
{
	_rb->addAngularForce(rotation);
}
void TankRemoteController::applyGunRotation(float rotation) 
{
	_targetGunRotation = rotation;
}
void TankRemoteController::applyShoot(bool shoot) const
{
	if (shoot) _tank->shoot();
}

void TankRemoteController::fixedUpdate()
{
	updateGunRotation();
}
void TankRemoteController::updateGunRotation() const
{
	auto currRot = _tank->_gunPivot->rot();
	auto newRot = Math::rotateTowardsAngle(currRot, _targetGunRotation, _gunRotSpeed * Time::fixedDeltaTime);
	if (!Math::nearlyEqual(newRot, currRot))
		_tank->_gunPivot->setRot(newRot);
}

void TankRemoteController::requestMove(float movement) { _requestedMovement = movement; }
void TankRemoteController::requestRotate(float rotation) { _requestedRotation = rotation; }
void TankRemoteController::requestGunTargetRotation(float rotation) { _requestedGunTargetRotation = rotation; }
void TankRemoteController::requestShoot() { _requestShoot = true; }

float TankRemoteController::getAndResetRequestedMovement()
{
	auto movement = _requestedMovement;
	_requestedMovement = 0;
	return movement;
}
float TankRemoteController::getAndResetRequestedRotation()
{
	auto rotation = _requestedRotation;
	_requestedRotation = 0;
	return rotation;
}
float TankRemoteController::getAndResetRequestedGunRotation()
{
	auto rotation = _requestedGunTargetRotation;
	_requestedGunTargetRotation = 0;
	return rotation;
}
bool TankRemoteController::getAndResetRequestShoot()
{
	auto shoot = _requestShoot;
	_requestShoot = false;
	return shoot;
}
