#include "TankController.h"

#include "Input.h"
#include "GameObject.h"
#include "glm/vec2.hpp"
#include "Rigidbody.h"
#include "Transform.h"

TankController::TankController(GameObject* obj, float moveSpeed, float rotationSpeed): Component(obj), moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
{
	rb = obj->getComponent<Rigidbody>();
}

void TankController::fixedUpdate()
{
	float moveDir = 0;
	if (Input::isKeyDown(SDLK_w)) moveDir += 1;
	if (Input::isKeyDown(SDLK_s)) moveDir -= 1;

	float rotDir = 0;
	if (Input::isKeyDown(SDLK_a)) rotDir += 1;
	if (Input::isKeyDown(SDLK_d)) rotDir -= 1;

	float speedMult = Input::isKeyDown(SDLK_LSHIFT) ? 3 : 1;
	if (moveDir != 0) rb->addForce(transform()->up() * moveDir * moveSpeed * speedMult);
	if (rotDir != 0) rb->addAngularForce(rotDir * rotationSpeed);
}
