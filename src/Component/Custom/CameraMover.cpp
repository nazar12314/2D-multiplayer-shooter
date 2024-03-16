#include "CameraMover.h"

#include "Input.h"
#include "MyTime.h"
#include "Object.h"
#include "glm/geometric.hpp"
#include "glm/vec2.hpp"

void CameraMover::update()
{
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
	obj->setPos(obj->pos + normalize(dir) * speed * Time::deltaTime);
}
