#pragma once

#include <vector>

#include "Component.h"
#include "glm/vec2.hpp"
#include "glm/detail/func_packing_simd.inl"

class Camera : public Component
{
	inline static std::vector<Camera*> cameras{};

	int _size;

	Camera(Object* obj, int size = 10);
	~Camera() override;

public:
	int priority = 0;

	int size() const;

	glm::vec2 screenToWorldPos(const glm::ivec2& screenPos) const;
	glm::ivec2 worldToScreenPos(const glm::vec2& worldPos) const;

	static Camera* getMain();

	friend class Object;
};
