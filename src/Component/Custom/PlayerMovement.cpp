#include "PlayerMovement.h"

#include <iostream>

#include "Input.h"
#include "MyTime.h"
#include "Object.h"
#include "PolygonCollider.h"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/vec2.hpp"

void PlayerMovement::update()
{
	auto myCol = obj->getComponent<PolygonCollider>();
	std::cout << col->collidesWith(myCol) << std::endl;

	glm::vec2 dir {};
	if (Input::isKeyDown(SDLK_w))
		dir.y += 1;
	if (Input::isKeyDown(SDLK_s))
		dir.y -= 1;
	if (Input::isKeyDown(SDLK_a))
		dir.x -= 1;
	if (Input::isKeyDown(SDLK_d))
		dir.x += 1;

	if (dir == glm::vec2(0, 0)) return;
	obj->setPos(obj->pos() + normalize(dir) * speed * Time::deltaTime);
	obj->setRot(glm::degrees(atan2(dir.y, dir.x)));
}
