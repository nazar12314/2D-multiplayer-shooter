#include "Camera.h"

#include "Application.h"
#include "Graphical.h"

Camera::Camera(int size, const glm::vec2& pos, float rot): Object(pos, rot), size(size)
{
	cameras.emplace_back(this);
}
Camera::~Camera()
{
	cameras.erase(std::ranges::find(cameras, this));
}

void Camera::render() const
{
	for (auto obj : Graphical::graphicalObjects)
	{
		if (!obj->enabled) continue;
		obj->draw(pos, size);
	}
}

void Camera::renderMain()
{
	if (cameras.empty()) return;

	// Find camera with highest priority
	auto lambda = [](Object* a, Object* b) { return ((Camera*)a)->priority < ((Camera*)b)->priority; };
	auto targetCamera = (Camera*)*std::max_element(cameras.begin(), cameras.end(), lambda);

	targetCamera->render();
}
