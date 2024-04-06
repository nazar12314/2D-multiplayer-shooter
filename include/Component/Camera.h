#pragma once

#include <vector>

#include "Component.h"
#include "glm/vec2.hpp"
#include "glm/detail/func_packing_simd.inl"

class Camera : public Component
{
	inline static std::vector<Camera*> cameras {};

	float _size;

	Camera(GameObject* obj, float size = 10);
	~Camera() override;

public:
	int priority = 0;

	float size() const;
	void setSize(float size);

	glm::vec2 screenToWorldPoint(const glm::vec2& screenPos) const;
	glm::vec2 worldToScreenPoint(const glm::vec2& worldPos) const;
	float screenToWorldSize(float screenSize) const;
	float worldToScreenSize(float worldSize) const;
	glm::vec2 screenToWorldSize(const glm::vec2& screenSize) const;
	glm::vec2 worldToScreenSize(const glm::vec2& worldSize) const;

	static Camera* getMain();

	friend class GameObject;
};
