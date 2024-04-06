#pragma once

#include "Component.h"

class CameraFollow : public Component
{
	GameObject* target = nullptr;

	CameraFollow(GameObject* obj, float smoothness);

	void fixedUpdate() override;

public:
	float smoothness;

	void setTarget(GameObject* obj) { target = obj; }

	friend class GameObject;
};
