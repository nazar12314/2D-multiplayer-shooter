#pragma once

#include "Component.h"
#include "Color.h"
#include "Renderer.h"

class Texture;
class Camera;

class BaseRenderer : public Component
{
protected:
	glm::vec2 _size;
	Color _color = Color::white;
	int _order = 0;
	bool _preserveAspect = false;

	Texture* _texture = nullptr;

	BaseRenderer(GameObject* obj, glm::vec2 size = {1, 1}, const Color& color = Color::white, int order = 0);

	void render(const Camera* camera) const;

	glm::vec2 getFinalSize() const;

public:
	~BaseRenderer() override;

	glm::vec2 size() const;
	Color color() const;
	int order() const;
	Texture* texture() const;

	void setSize(const glm::vec2& size);
	void setOrder(int order);
	virtual void setColor(const Color& color);
	void setPreserveAspect(bool preserveAspect);

	void setNativeSize();

	friend class GameObject;
	friend class Renderer;
};
