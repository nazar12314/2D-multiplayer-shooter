#include "TankRemoteController.h"

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
void TankRemoteController::rotateTo(float angle) const
{
	_rb->rotateTo(angle);
}
void TankRemoteController::rotateGunTo(float angle) const
{
	_tank->_gunPivot->setRot(angle);
}
void TankRemoteController::shoot() const
{
	_tank->shoot();
}
