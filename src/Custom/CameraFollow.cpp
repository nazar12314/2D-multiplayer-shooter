#include "CameraFollow.h"

#include "MyMath.h"
#include "MyTime.h"
#include "GameObject.h"
#include "Transform.h"

CameraFollow::CameraFollow(GameObject* obj, float smoothness): Component(obj), _smoothness(smoothness) { }

void CameraFollow::fixedUpdate()
{
	if (target == nullptr) return;

	auto newPos = mix(transform()->pos(), target->pos(), _smoothness * Time::fixedDeltaTime);
	if (Math::nearlyEqual(transform()->pos(), newPos)) return;
	transform()->setPos(newPos);
}
void CameraFollow::setTarget(Transform* obj)
{
	target = obj;
}
