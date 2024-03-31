#include "CameraResizer.h"

#include "Camera.h"
#include "Input.h"
#include "Math.h"
#include "MyTime.h"
#include "glm/common.hpp"

CameraResizer::CameraResizer(Object* owner, float startSize, float sensitivity, float smoothness) : Component(owner),
                                                                                                    targetSize(startSize), sensitivity(sensitivity), smoothness(smoothness) {}

void CameraResizer::update()
{
	auto cam = Camera::getMain();
	if (Input::mouseWheelValue != 0)
		targetSize = glm::clamp(targetSize - sensitivity * Input::mouseWheelValue, 5.0f, 50.0f);

	cam->setSize(Math::lerp(cam->size(), targetSize, smoothness * Time::deltaTime));
}
