#include "CameraFollow.h"

#include "MyMath.h"
#include "MyTime.h"
#include "GameObject.h"
#include "Transform.h"

CameraFollow::CameraFollow(GameObject* obj, float smoothness): Component(obj), _smoothness(smoothness) { }

void CameraFollow::fixedUpdate()
{
	if (target == nullptr) return;

	auto newPos = mix(transform()->getPos(), target->transform()->getPos(), _smoothness * Time::fixedDeltaTime);
	if (Math::nearlyEqual(transform()->getPos(), newPos)) return;
	transform()->setPos(newPos);
}
void CameraFollow::setTarget(GameObject* obj)
{
	target = obj;
}
