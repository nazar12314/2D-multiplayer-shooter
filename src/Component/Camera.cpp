#include "Camera.h"

#include "Object.h"
#include "SDLHandler.h"
#include "SpriteRenderer.h"

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

glm::vec2 Camera::screenToWorldPoint(const glm::vec2& screenPos) const
{
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	auto ratio = wSize.x / wSize.y;
	auto relPos = glm::vec2(screenPos.x / wSize.y - ratio / 2, 0.5f - screenPos.y / wSize.y);
	return relPos * _size + obj->pos();
}

glm::vec2 Camera::worldToScreenPoint(const glm::vec2& worldPos) const
{
	auto relPos = (worldPos - obj->pos()) / _size;
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	auto ratio = wSize.x / wSize.y;
	return glm::vec2(relPos.x + ratio / 2, 0.5f - relPos.y) * wSize.y;
}
float Camera::screenToWorldSize(float screenSize) const
{
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	return screenSize / wSize.y * _size;
}
float Camera::worldToScreenSize(float worldSize) const
{
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	return worldSize / _size * wSize.y;
}
glm::vec2 Camera::screenToWorldSize(const glm::vec2& screenSize) const
{
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	return screenSize / wSize.y * _size;
}
glm::vec2 Camera::worldToScreenSize(const glm::vec2& worldSize) const
{
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	return (worldSize / _size) * wSize.y;
}

Camera* Camera::getMain()
{
	if (cameras.empty()) return nullptr;
	auto lambda = [](const Camera* a, const Camera* b) { return a->priority < b->priority; };
	return *std::ranges::max_element(cameras, lambda);
}
