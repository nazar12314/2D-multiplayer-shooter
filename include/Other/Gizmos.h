#pragma once

#include <vector>

#include "Color.h"
#include "VectorDelayed.h"
#include "glm/vec2.hpp"

class Gizmo;

class Gizmos
{
	inline static VectorDelayed<Gizmo*> gizmos {};

	static void draw();

public:
	static void drawLine(glm::vec2 p1, glm::vec2 p2, const Color& color = Color::green, float dur = 0.01f);
	static void drawRect(glm::vec2 pos, glm::vec2 size, const Color& color = Color::green, float dur = 0.01f);
	static void drawCircle(glm::vec2 pos, float radius, const Color& color = Color::green, float dur = 0.01f);
	static void drawPoint(glm::vec2 pos, float radius, const Color& color = Color::green, float dur = 0.01f);

	friend class Renderer;
	friend class Gizmo;
};

class Gizmo
{
protected:
	Color color;
	float duration;

	Gizmo(const Color& color, float duration);

	virtual void draw() const = 0;

	friend class Gizmos;
};

class PolygonGizmo : public Gizmo
{
	std::vector<glm::vec2> points;

	PolygonGizmo(const std::vector<glm::vec2>& points, const Color& color, float duration);

	void draw() const override;

	friend class Gizmos;
};

class PointGizmo : public Gizmo
{
	glm::vec2 pos;
	float radius;

	PointGizmo(glm::vec2 pos, float radius, const Color& color, float duration);

	void draw()const override;

	friend class Gizmos;
};
