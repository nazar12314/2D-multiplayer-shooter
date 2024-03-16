#pragma once

#include "Component.h"

class Camera : public Component
{
	Camera(Object* obj, int size = 10);

public:
	int size;
	int priority = 0;

	friend class Object;
};
