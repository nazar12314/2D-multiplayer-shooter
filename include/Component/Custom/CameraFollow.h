#pragma once

#include "Component.h"

class CameraFollow : public Component
{
	Object* target = nullptr;

	CameraFollow(Object* obj, float smoothness);

	void lateUpdate() override;

public:
	float smoothness;

	void setTarget(Object* obj) { target = obj; }

	friend class Object;
};
