#include "Camera.h"

#include <algorithm>

#include "GameObject.h"
#include "SDLHandler.h"
#include "SpriteRenderer.h"
#include "Transform.h"

Camera::Camera(GameObject* obj, float size, const Color& bgColor): Component(obj), _size(size), _bgColor(bgColor)
{
	cameras.push_back(this);
}
Camera::~Camera()
{
	std::erase(cameras, this);
}

float Camera::size() const { return _size; }
int Camera::priority() const { return _priority; }
Color Camera::bgColor() const { return _bgColor; }

void Camera::setSize(float size) { _size = size; }
void Camera::setPriority(int priority) { _priority = priority; }
void Camera::setBgColor(const Color& color) { _bgColor = color; }

glm::vec2 Camera::screenToWorldPoint(const glm::vec2& screenPos) const
{
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	auto relPos = glm::vec2(screenPos.x / wSize.y - SDLHandler::windowAspectRatio / 2, 0.5f - screenPos.y / wSize.y);
	auto localPos = relPos * _size;
	return transform()->localToGlobalPos(localPos);
}

glm::vec2 Camera::worldToScreenPoint(const glm::vec2& worldPos) const
{
	auto localPos = transform()->globalToLocalPos(worldPos);
	auto relPos = localPos / _size;
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	return glm::vec2(relPos.x + SDLHandler::windowAspectRatio / 2, 0.5f - relPos.y) * wSize.y;
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
	auto lambda = [](const Camera* a, const Camera* b) { return a->_priority < b->_priority; };
	return *std::ranges::max_element(cameras, lambda);
}
