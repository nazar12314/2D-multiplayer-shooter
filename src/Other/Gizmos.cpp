#include "Gizmos.h"

#include "MyTime.h"
#include "Renderer.h"

void Gizmos::draw()
{
	gizmos.apply_changes();
	for (auto& gizmo : gizmos)
	{
		gizmo->draw();

		gizmo->duration -= Time::deltaTime;
		if (gizmo->duration <= 0)
		{
			gizmos.erase_delayed(gizmo);
			delete gizmo;
		}
	}
}

void Gizmos::drawLine(glm::vec2 p1, glm::vec2 p2, const Color& color, float dur)
{
	gizmos.push_back(new PolygonGizmo({p1, p2}, color, dur));
}
void Gizmos::drawRect(glm::vec2 pos, glm::vec2 size, const Color& color, float dur)
{
	auto p1 = pos;
	auto p2 = pos + glm::vec2(size.x, 0);
	auto p3 = pos + size;
	auto p4 = pos + glm::vec2(0, size.y);
	gizmos.push_back(new PolygonGizmo({p1, p2, p3, p4, p1}, color, dur));
}

void Gizmos::drawCircle(glm::vec2 pos, float radius, const Color& color, float dur)
{
	std::vector<glm::vec2> points;
	for (int i = 0; i < 360; i += 10)
	{
		float angle = glm::radians((float)i);
		points.push_back(pos + glm::vec2(cos(angle), sin(angle)) * radius);
	}
	gizmos.push_back(new PolygonGizmo(points, color, dur));
}
void Gizmos::drawPoint(glm::vec2 pos, float radius, const Color& color, float dur)
{
	gizmos.push_back(new PointGizmo(pos, radius, color, dur));
}


Gizmo::Gizmo(const Color& color, float duration): color(color), duration(duration) {}

PolygonGizmo::PolygonGizmo(const std::vector<glm::vec2>& points, const Color& color, float duration) : Gizmo(color, duration), points(points) {}
void PolygonGizmo::draw() const
{
	for (int i = 0; i < points.size() - 1; i++)
	{
		auto p1 = points[i];
		auto p2 = points[i + 1];
		Renderer::drawLine(p1, p2, color);
	}
}

PointGizmo::PointGizmo(glm::vec2 pos, float radius, const Color& color, float duration) : Gizmo(color, duration), pos(pos), radius(radius) {}
void PointGizmo::draw() const
{
	Renderer::drawCircle(pos, radius + 0.02f, Color::white);
	Renderer::drawCircle(pos, radius, color);
}
