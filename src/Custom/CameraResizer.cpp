#include "CameraResizer.h"

#include "Camera.h"
#include "Input.h"
#include "MyMath.h"
#include "MyTime.h"
#include "GameObject.h"
#include "Transform.h"
#include "glm/common.hpp"

CameraResizer::CameraResizer(GameObject* owner, float sensitivity, float smoothness, bool zoomTowardsMouse) : Component(owner),
	sensitivity(sensitivity), smoothness(smoothness), zoomTowardsMouse(zoomTowardsMouse)
{
	targetSize = Camera::getMain()->size();
}

void CameraResizer::update()
{
	auto cam = Camera::getMain();
	auto prevSize = cam->size();

	//targetSize = glm::clamp(targetSize - sensitivity * Input::mouseWheelValue, 0.1f, 50.0f);
	targetSize -= targetSize / 10 * sensitivity * Input::mouseWheelValue();
	targetSize = glm::clamp(targetSize, MIN_SIZE, MAX_SIZE);
	cam->setSize(Math::lerp(cam->size(), targetSize, smoothness * Time::deltaTime()));

	if (!zoomTowardsMouse) return;
	auto mouseWorldPos = cam->screenToWorldPoint(Input::mousePos());
	cam->transform()->setPos(cam->transform()->pos() + (prevSize - cam->size()) * (mouseWorldPos - cam->transform()->pos()) / prevSize);
}
