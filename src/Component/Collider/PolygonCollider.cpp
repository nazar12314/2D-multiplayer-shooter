#include "PolygonCollider.h"

#include "CircleCollider.h"
#include <cmath>
#include <vector>
#include <glm/glm.hpp>

bool PolygonCollider::collidesWith(Collider* other)
{
	float sRot = glm::radians(45.5f);

	glm::vec2 sPos = obj->getPos();
	glm::vec2 oPos = other->obj->getPos();

	if (const auto* box = dynamic_cast<const PolygonCollider*>(other))
	{
        for (int i = 0; i < edges.size(); i++)
        {
            glm::vec2 edge = edges[i];
            glm::vec2 nextEdge = edges[(i + 1) % edges.size()];

            glm::vec2 axis = {edge.y - nextEdge.y, nextEdge.x - edge.x};

            float minS = std::numeric_limits<float>::max();
            float maxS = std::numeric_limits<float>::min();
            float minO = std::numeric_limits<float>::max();
            float maxO = std::numeric_limits<float>::min();

            for (auto & edg : edges)
            {
                float proj = (edg.x * axis.x + edg.y * axis.y) / (axis.x * axis.x + axis.y * axis.y);
                minS = std::min(minS, proj);
                maxS = std::max(maxS, proj);
            }

            for (auto edg : box->edges)
            {
                float proj = (edg.x * axis.x + edg.y * axis.y) / (axis.x * axis.x + axis.y * axis.y);
                minO = std::min(minO, proj);
                maxO = std::max(maxO, proj);
            }

            if (maxS < minO || maxO < minS) return false;
        }

        return true;
	}

	if (const auto* circle = dynamic_cast<const CircleCollider*>(other))
	{
		double unrotatedX = std::cos(sRot)
			* (oPos.x - sPos.x) - std::sin(sRot)
			* (oPos.y - sPos.y) + sPos.x;

		double unrotatedY = std::sin(sRot)
			* (oPos.x - sPos.x) + std::cos(sRot)
			* (oPos.y - sPos.y) + sPos.y;

		double closestX, closestY;

        float rectX = sPos.x - (size.x / 2);
        float rectY = sPos.y - (size.y / 2);

        // Rectangle size as width x height
		if (unrotatedX < rectX)
			closestX = rectX;
		else if (unrotatedX > rectX + size.x)
			closestX = rectX + size.x;
		else
			closestX = unrotatedX;

        if (unrotatedY < rectY)
            closestY = rectY;
        else if (unrotatedY > rectY + size.y)
            closestY = rectY + size.y;
        else
            closestY = unrotatedY;

		double distance = findDistance(unrotatedX, unrotatedY, closestX, closestY);

		if (distance < circle->radius) return true;
	}

	return false;
}

double PolygonCollider::findDistance(double startX, double startY, double endX, double endY)
{
	double a = std::abs(startX - endX);
	double b = std::abs(startY - endY);

	return std::sqrt(a * a + b * b);
}
