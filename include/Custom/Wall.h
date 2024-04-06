#pragma once

#include "Component.h"
#include "PolygonCollider.h"


class Wall : public Component
{
	explicit Wall(GameObject* obj, glm::vec2 size);

	friend class GameObject;
};
