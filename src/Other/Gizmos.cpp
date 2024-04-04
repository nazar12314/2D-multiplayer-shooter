#include "Gizmos.h"

#include "Camera.h"
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

Gizmo* Gizmos::drawLine(glm::vec2 p1, glm::vec2 p2, const Color& color, float dur)
{
	auto gizmo = new PolygonGizmo({p1, p2}, color, dur);
	gizmos.push_back(gizmo);

	return gizmo;
}
Gizmo* Gizmos::drawRect(glm::vec2 pos, glm::vec2 size, const Color& color, float dur)
{
	auto p1 = pos;
	auto p2 = pos + glm::vec2(size.x, 0);
	auto p3 = pos + size;
	auto p4 = pos + glm::vec2(0, size.y);

	auto gizmo = new PolygonGizmo({p1, p2, p3, p4, p1}, color, dur);
	gizmos.push_back(gizmo);

	return gizmo;
}

Gizmo* Gizmos::drawCircle(glm::vec2 pos, float radius, const Color& color, float dur)
{
	std::vector<glm::vec2> points;
	for (int i = 0; i < 360; i += 10)
	{
		float angle = glm::radians((float)i);
		points.push_back(pos + glm::vec2(cos(angle), sin(angle)) * radius);
	}
	auto gizmo = new PolygonGizmo(points, color, dur);
	gizmos.push_back(gizmo);

	return gizmo;
}
Gizmo* Gizmos::drawPoint(glm::vec2 pos, float radius, const Color& color, float dur)
{
	auto gizmo = new PointGizmo(pos, radius, color, dur);
	gizmos.push_back(gizmo);

	return gizmo;
}
Gizmo* Gizmos::drawVector(glm::vec2 pos, glm::vec2 dir, float length, const Color& color, float dur)
{
	auto gizmo = new VectorGizmo(pos, dir, length, color, dur);
	gizmos.push_back(gizmo);

	return gizmo;
}
void Gizmos::remove(Gizmo* gizmo)
{
	std::erase(gizmos, gizmo);
	delete gizmo;
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
	auto sizeMult = Camera::getMain()->size();
	Renderer::drawCircleWorld(pos, sizeMult * (radius + 0.015f) / 7.0f, Color::white);
	Renderer::drawCircleWorld(pos, sizeMult * radius / 7, color);
}

VectorGizmo::VectorGizmo(glm::vec2 pos, glm::vec2 dir, float length, const Color& color, float duration) : Gizmo(color, duration), pos(pos), dir(dir), length(length) {}
void VectorGizmo::draw() const
{
	auto sizeMult = Camera::getMain()->size();
	Renderer::drawLine(pos, pos + sizeMult * dir * length / 15.0f, color, sizeMult / 300.0f);
}
