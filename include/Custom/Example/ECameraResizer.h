#pragma once

#include "Component.h"

namespace examples
{
	class ECameraResizer : public Component
	{
		float targetSize;
		float sensitivity;
		float smoothness;
		bool zoomTowardsMouse;

		ECameraResizer(GameObject* owner, float startSize, float sensitivity = 1.0f, float smoothness = 1.0f, bool zoomTowardsMouse = false);

	public:
		void update() override;

		friend class ::GameObject;
	};
}
