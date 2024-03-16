#include "CameraFollow.h"

#include "MyTime.h"
#include "Object.h"
#include "glm/common.hpp"

CameraFollow::CameraFollow(Object* obj, float smoothness): Component(obj), smoothness(smoothness) {}

void CameraFollow::fixedUpdate()
{
	if (target == nullptr) return;
	obj->setPos(mix(obj->pos(), target->pos(), smoothness * Time::fixedDeltaTime));
}
