#include "ECameraResizer.h"

#include "Camera.h"
#include "Input.h"
#include "MyMath.h"
#include "MyTime.h"
#include "GameObject.h"
#include "Transform.h"
#include "glm/common.hpp"

namespace examples
{
	ECameraResizer::ECameraResizer(GameObject* owner, float startSize, float sensitivity, float smoothness, bool zoomTowardsMouse) : Component(owner), targetSize(startSize),
		sensitivity(sensitivity), smoothness(smoothness), zoomTowardsMouse(zoomTowardsMouse)
	{
		Camera::getMain()->setSize(startSize);
	}

	void ECameraResizer::update()
	{
		auto cam = Camera::getMain();

		//targetSize = glm::clamp(targetSize - sensitivity * Input::mouseWheelValue, 0.1f, 50.0f);
		targetSize -= targetSize / 10 * sensitivity * Input::mouseWheelValue();
		auto prevSize = cam->size();
		cam->setSize(Math::lerp(cam->size(), targetSize, smoothness * Time::deltaTime()));

		if (!zoomTowardsMouse) return;
		auto mouseWorldPos = cam->screenToWorldPoint(Input::mousePos());
		cam->transform()->setPos(cam->transform()->pos() + (prevSize - cam->size()) * (mouseWorldPos - cam->transform()->pos()) / prevSize);
	}
}
