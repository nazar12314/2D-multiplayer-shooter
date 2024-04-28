#pragma once

#include "Component.h"

class CameraFollow : public Component
{
	float _smoothness;
	Transform* target = nullptr;

	CameraFollow(GameObject* obj, float smoothness);

	void fixedUpdate() override;

public:
	void setTarget(Transform* obj);

	friend class GameObject;
};
