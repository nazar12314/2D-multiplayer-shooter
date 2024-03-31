#pragma once

#include <vector>

#include "Component.h"
#include "glm/vec2.hpp"
#include "glm/detail/func_packing_simd.inl"

class Camera : public Component
{
	inline static std::vector<Camera*> cameras{};

	float _size;

	Camera(Object* obj, float size = 10);
	~Camera() override;

public:
	int priority = 0;

	float size() const;
	void setSize(float size);

	glm::vec2 screenToWorldPos(const glm::ivec2& screenPos) const;
	glm::ivec2 worldToScreenPos(const glm::vec2& worldPos) const;

	static Camera* getMain();

	friend class Object;
};