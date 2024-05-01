#include "ECameraFollow.h"

#include "MyMath.h"
#include "MyTime.h"
#include "GameObject.h"
#include "Transform.h"

namespace examples
{
	ECameraFollow::ECameraFollow(GameObject* obj, float smoothness): Component(obj), _smoothness(smoothness) { }

	void ECameraFollow::fixedUpdate()
	{
		if (target == nullptr) return;

		auto newPos = mix(transform()->pos(), target->transform()->pos(), _smoothness * Time::fixedDeltaTime);
		if (Math::nearlyEqual(transform()->pos(), newPos)) return;
		transform()->setPos(newPos);
	}
	void ECameraFollow::setTarget(GameObject* obj)
	{
		target = obj;
	}
}
