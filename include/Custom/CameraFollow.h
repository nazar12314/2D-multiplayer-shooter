#pragma once

#include "Component.h"

class CameraFollow : public Component
{
	float _smoothness;
	GameObject* target = nullptr;

	CameraFollow(GameObject* obj, float smoothness);

	void fixedUpdate() override;

public:
	void setTarget(GameObject* obj);

	friend class GameObject;
};
