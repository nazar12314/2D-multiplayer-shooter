#pragma once

#include "Component.h"

class Collider;
class Rigidbody;

namespace examples
{
	class EBullet : public Component
	{
		float _speed;
		Rigidbody* rb;
		bool _explode;

		EBullet(GameObject* obj, float speed = 16, bool explode = false);

		void fixedUpdate() override;
		void onTriggerEnter(Collider* other) override;

		friend class ::GameObject;
	};
}
