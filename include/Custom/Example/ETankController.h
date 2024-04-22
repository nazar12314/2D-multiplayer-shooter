#pragma once

#include "Component.h"

class Rigidbody;
class PolygonCollider;

namespace examples
{
	class ETankController : public Component
	{
		Rigidbody* rb = nullptr;

		ETankController(GameObject* obj, float moveSpeed = 130, float rotationSpeed = 80);

		void fixedUpdate() override;

	public:
		float moveSpeed;
		float rotationSpeed;

		friend class ::GameObject;
	};
}
