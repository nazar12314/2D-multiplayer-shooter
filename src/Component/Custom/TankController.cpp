#include "TankController.h"

#include "Input.h"
#include "MyTime.h"
#include "Object.h"
#include "PolygonCollider.h"
#include "glm/vec2.hpp"
#include "RigidBody.h"

TankController::TankController(Object* obj, float moveSpeed, float rotationSpeed): Component(obj), moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
{
	rb = obj->getComponent<RigidBody>();
}

void TankController::fixedUpdate()
{
	float moveDir = 0;
	float rotDir = 0;
	if (Input::isKeyDown(SDLK_w)) moveDir += 1;
	if (Input::isKeyDown(SDLK_s)) moveDir -= 1;
	if (Input::isKeyDown(SDLK_a)) rotDir -= 1;
	if (Input::isKeyDown(SDLK_d)) rotDir += 1;

	float speedMult = Input::isKeyDown(SDLK_LSHIFT) ? 3 : 1;
	if (moveDir != 0) rb->addVelocity(obj->up() * moveDir * moveSpeed * speedMult * Time::fixedDeltaTime);
	if (rotDir != 0) rb->addAngularVelocity(rotDir * rotationSpeed * Time::fixedDeltaTime);
}
