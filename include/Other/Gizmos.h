#pragma once

#include <vector>

#include "Color.h"
#include "VectorSafe.h"
#include "glm/vec2.hpp"

class Gizmo;

class Gizmos
{
	inline static VectorSafe<Gizmo*> gizmos {};

	static void draw();

public:
	static Gizmo* drawLine(glm::vec2 p1, glm::vec2 p2, const Color& color = Color::GREEN, float dur = 0.01f);
	static Gizmo* drawRect(glm::vec2 pos, glm::vec2 size, const Color& color = Color::GREEN, float dur = 0.01f);
	static Gizmo* drawCircle(glm::vec2 pos, float radius, const Color& color = Color::GREEN, float dur = 0.01f);
	static Gizmo* drawPoint(glm::vec2 pos, float radius, const Color& color = Color::GREEN, float dur = 0.01f);
	static Gizmo* drawVector(glm::vec2 pos, glm::vec2 dir, float length = 1, const Color& color = Color::GREEN, float dur = 0.01f);

	static void remove(Gizmo* gizmo);

	friend class Renderer;
	friend class Gizmo;
};

class Gizmo
{
protected:
	Color color;
	float duration;

	Gizmo(const Color& color, float duration);
	virtual ~Gizmo() = default;

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

	void draw() const override;

	friend class Gizmos;
};

class VectorGizmo : public Gizmo
{
	glm::vec2 pos;
	glm::vec2 dir;
	float length;

	VectorGizmo(glm::vec2 pos, glm::vec2 dir, float length, const Color& color, float duration);

	void draw() const override;

	friend class Gizmos;
};
