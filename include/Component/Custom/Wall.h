#pragma once

#include "Component.h"
#include "PolygonCollider.h"


class Wall : public Component
{
	explicit Wall(Object* obj, glm::vec2 size);

	friend class Object;
};
