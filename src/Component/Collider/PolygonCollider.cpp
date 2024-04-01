#include "PolygonCollider.h"

#include <cmath>
#include <vector>

#include "Math.h"
#include "SpriteRenderer.h"
#include "Assets.h"
#include "CircleCollider.h"
#include "Object.h"
#include "glm/gtx/rotate_vector.hpp"

PolygonCollider::PolygonCollider(Object* obj, glm::vec2 size, bool isTrigger): Collider(obj, isTrigger), _size(size)
{
	for (int i = 0; i < 4; i++)
		edgePointsTEST.push_back(Object::create("pointTEMP")->addComponent<SpriteRenderer>(Assets::load<Texture>("sprites/circle.png"), glm::vec2(0.1f, 0.1f)));
}

bool PolygonCollider::intersectsWith(Collider* other)
{
	glm::vec2 sPos = obj->pos();
	glm::vec2 oPos = other->obj->pos();

	if (const auto* box = dynamic_cast<const PolygonCollider*>(other))
	{
		for (int i = 0; i < _edges.size(); i++)
		{
			glm::vec2 edge = _edges[i];
			glm::vec2 nextEdge = _edges[(i + 1) % _edges.size()];

			glm::vec2 axis = {edge.y - nextEdge.y, nextEdge.x - edge.x};

			float minS = std::numeric_limits<float>::max();
			float maxS = std::numeric_limits<float>::min();
			float minO = std::numeric_limits<float>::max();
			float maxO = std::numeric_limits<float>::min();

			for (auto& edg : _edges)
			{
				float proj = (edg.x * axis.x + edg.y * axis.y) / (axis.x * axis.x + axis.y * axis.y);
				minS = std::min(minS, proj);
				maxS = std::max(maxS, proj);
			}

			for (auto edg : box->_edges)
			{
				float proj = (edg.x * axis.x + edg.y * axis.y) / (axis.x * axis.x + axis.y * axis.y);
				minO = std::min(minO, proj);
				maxO = std::max(maxO, proj);
			}

			if (maxS < minO || maxO < minS) return false;
		}

		return true;
	}

	float sRot = glm::radians(obj->rot());
	if (const auto* circle = dynamic_cast<const CircleCollider*>(other))
	{
		double unrotatedX = std::cos(sRot)
			* (oPos.x - sPos.x) - std::sin(sRot)
			* (oPos.y - sPos.y) + sPos.x;

		double unrotatedY = std::sin(sRot)
			* (oPos.x - sPos.x) + std::cos(sRot)
			* (oPos.y - sPos.y) + sPos.y;

		double closestX, closestY;

		float rectX = sPos.x - (_size.x / 2);
		float rectY = sPos.y - (_size.y / 2);

		// Rectangle size as width x height
		if (unrotatedX < rectX)
			closestX = rectX;
		else if (unrotatedX > rectX + _size.x)
			closestX = rectX + _size.x;
		else
			closestX = unrotatedX;

		if (unrotatedY < rectY)
			closestY = rectY;
		else if (unrotatedY > rectY + _size.y)
			closestY = rectY + _size.y;
		else
			closestY = unrotatedY;

		double distance = Math::distance(unrotatedX, unrotatedY, closestX, closestY);
		if (distance < circle->radius) return true;
	}
	return false;
}


void PolygonCollider::recalculate()
{
	glm::vec2 sPos = obj->pos();
	float sRot = glm::radians(obj->rot());

	_edges = {
		{sPos.x - _size.x / 2, sPos.y - _size.y / 2}, // bottom left
		{sPos.x - _size.x / 2, sPos.y + _size.y / 2}, // top left
		{sPos.x + _size.x / 2, sPos.y + _size.y / 2}, // top right
		{sPos.x + _size.x / 2, sPos.y - _size.y / 2}, // bottom right
	};

	for (auto& edge : _edges)
		edge = rotate(edge - sPos, -sRot) + sPos;

	for (int i = 0; i < 4; i++)
		edgePointsTEST[i]->obj->setPos(_edges[i]);
}
void PolygonCollider::onDestroy()
{
	for (auto& point : edgePointsTEST)
		Object::destroy(point->obj);
}
