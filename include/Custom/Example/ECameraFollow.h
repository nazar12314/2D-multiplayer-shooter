#pragma once

#include "Component.h"

namespace examples
{
	class ECameraFollow : public Component
	{
		float _smoothness;
		GameObject* target = nullptr;

		ECameraFollow(GameObject* obj, float smoothness);

		void fixedUpdate() override;

	public:
		void setTarget(GameObject* obj);

		friend class GameObject;
	};
}
