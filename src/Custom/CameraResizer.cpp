#include "CameraResizer.h"

#include "Camera.h"
#include "Input.h"
#include "MyMath.h"
#include "MyTime.h"
#include "Object.h"
#include "glm/common.hpp"

CameraResizer::CameraResizer(Object* owner, float startSize, float sensitivity, float smoothness, bool zoomTowardsMouse) : Component(owner), targetSize(startSize),
	sensitivity(sensitivity), smoothness(smoothness), zoomTowardsMouse(zoomTowardsMouse)
{
	Camera::getMain()->setSize(startSize);
}

void CameraResizer::update()
{
	auto cam = Camera::getMain();

	//targetSize = glm::clamp(targetSize - sensitivity * Input::mouseWheelValue, 0.1f, 50.0f);
	targetSize -= targetSize / 10 * sensitivity * Input::mouseWheelValue;
	auto prevSize = cam->size();
	cam->setSize(Math::lerp(cam->size(), targetSize, smoothness * Time::deltaTime));

	if (!zoomTowardsMouse) return;
	auto mouseWorldPos = cam->screenToWorldPoint(Input::mousePos);
	cam->obj->setPos(cam->obj->pos() + (prevSize - cam->size()) * (mouseWorldPos - cam->obj->pos()) / prevSize);
}
