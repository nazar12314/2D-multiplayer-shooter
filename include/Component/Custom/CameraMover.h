#pragma once 

#include "Component.h"

class CameraMover : public Component
{
public:
	float speed = 0;

	explicit CameraMover(Object* obj) : Component(obj) {}

private:
	void update() override;
};