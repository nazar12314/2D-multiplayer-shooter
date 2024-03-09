#include "Camera.h"

#include "Application.h"

Camera::Camera(const glm::vec2& pos, float rot, int size): Object(pos, rot), size(size) {}

void Camera::render() const
{
	for (auto obj : objects)
	{
		if (!obj->enabled)continue;

		auto graphical = dynamic_cast<Graphical*>(obj);
		if (graphical == nullptr) continue;

		graphical->draw(pos, size);
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
