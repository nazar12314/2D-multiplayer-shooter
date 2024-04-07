#pragma once

#include <vector>

#include "Color.h"
#include "Component.h"
#include "glm/vec2.hpp"

class Camera : public Component
{
	inline static std::vector<Camera*> cameras {};

	float _size;
	int _priority = 0;
	Color _bgColor;

	Camera(GameObject* obj, float size = 10, const Color& bgColor = Color::black);

public:
	~Camera() override;

	float size() const;
	int priority() const;
	Color bgColor() const;

	void setSize(float size);
	void setPriority(int priority);
	void setBgColor(const Color& color);

	glm::vec2 screenToWorldPoint(const glm::vec2& screenPos) const;
	glm::vec2 worldToScreenPoint(const glm::vec2& worldPos) const;
	float screenToWorldSize(float screenSize) const;
	float worldToScreenSize(float worldSize) const;
	glm::vec2 screenToWorldSize(const glm::vec2& screenSize) const;
	glm::vec2 worldToScreenSize(const glm::vec2& worldSize) const;

	static Camera* getMain();

	friend class GameObject;
	friend std::unique_ptr<Camera>::deleter_type;
};
