#pragma once

#include "Component.h"
#include "Color.h"
#include "Renderer.h"

class Tween;
class Texture;
class Camera;

class BaseRenderer : public Component
{
protected:
	Color _color = Color::WHITE;
	int _order = 0;

	Texture* _texture = nullptr;

	BaseRenderer(GameObject* obj, const Color& color = Color::WHITE, int order = 0);

	virtual void render(const Camera* camera) const = 0;

public:
	~BaseRenderer() override;

	Color color() const;
	int order() const;
	Texture* texture() const;

	void setOrder(int order);
	virtual void setColor(const Color& color);

	friend class GameObject;
	friend class Renderer;

	// DOTween
	Tween* doColor(const Color& endValue, float duration);
};

class RectRenderer : public BaseRenderer
{
protected:
	glm::vec2 _size;
	bool _preserveAspect = false;

	void render(const Camera* camera) const override;
	glm::vec2 getFinalSize() const;

public:
	RectRenderer(GameObject* obj, glm::vec2 size = {1, 1}, const Color& color = Color::WHITE, int order = 0);

	glm::vec2 size() const;
	void setSize(const glm::vec2& size);
	void setPreserveAspect(bool preserveAspect);

	void setNativeSize();
};
