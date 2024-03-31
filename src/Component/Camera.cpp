#include "Camera.h"

#include "Object.h"
#include "SDLHandler.h"
#include "Sprite.h"

Camera::Camera(Object* obj, float size): Component(obj), _size(size)
{
	cameras.push_back(this);
}
Camera::~Camera()
{
	std::erase(cameras, this);
}

float Camera::size() const { return _size; }
void Camera::setSize(float size) { _size = size; }

glm::vec2 Camera::screenToWorldPos(const glm::ivec2& screenPos) const
{
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	auto ratio = wSize.x / wSize.y;
	auto relPos = glm::vec2(screenPos.x / wSize.y - ratio / 2, 0.5f - screenPos.y / wSize.y);
	return relPos * _size + obj->pos();
}

glm::ivec2 Camera::worldToScreenPos(const glm::vec2& worldPos) const
{
	auto relPos = (worldPos - obj->pos()) / _size;
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	auto ratio = wSize.x / wSize.y;
	return glm::vec2(relPos.x + ratio / 2, 0.5f - relPos.y) * wSize.y;
}
Camera* Camera::getMain()
{
	if (cameras.empty()) return nullptr;
	auto lambda = [](const Camera* a, const Camera* b) { return a->priority < b->priority; };
	return *std::max_element(cameras.begin(), cameras.end(), lambda);
}