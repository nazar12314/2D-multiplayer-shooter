#pragma once

#include "Component.h"

class CameraResizer : public Component
{
	float targetSize;
	float sensitivity;
	float smoothness;

	CameraResizer(Object* owner, float startSize, float sensitivity = 1.0f, float smoothness = 1.0f);

public:
	void update() override;

	friend class Object;
};
