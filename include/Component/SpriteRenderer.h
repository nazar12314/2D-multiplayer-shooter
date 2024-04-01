#pragma once

#include "Component.h"
#include "Color.h"
#include "glm/vec2.hpp"

class Texture;
class Material;

class SpriteRenderer : public Component
{
	Material* _material;
	glm::vec2 _size;
	int _order = 0;

	SpriteRenderer(Object* obj, Texture* texture, glm::vec2 size = {1, 1}, int order = 0, const Color& color = Color::white);
	~SpriteRenderer() override;

public:
	Material* material() const;
	glm::vec2 size() const;
	int order() const;

	void setNativeSize();
	void setOrder(int order);
	void setColor(const Color& color) const;

	void onDestroy() override;

	friend class Object;
	friend class Renderer;
};
