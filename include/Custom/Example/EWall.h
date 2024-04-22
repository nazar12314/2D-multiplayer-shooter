#pragma once

#include "Component.h"
#include "PolygonCollider.h"

namespace examples
{
	class EWall : public Component
	{
		explicit EWall(GameObject* obj, glm::vec2 size);

		friend class ::GameObject;
	};
}
