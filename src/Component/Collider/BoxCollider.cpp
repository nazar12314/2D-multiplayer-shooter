#include "BoxCollider.h"

#include "CircleCollider.h"
#include "Object.h"
#include "glm/detail/func_trigonometric.inl"
#include <cmath>

bool BoxCollider::collidesWith(const Collider* other)
{
	float sRot = glm::radians(obj->getRot());

	glm::vec2 sPos = obj->getPos();
	glm::vec2 oPos = other->obj->getPos();

	if (const auto* box = dynamic_cast<const BoxCollider*>(other))
	{
		auto [sAxisX, sAxisY] = getAxis(this);
		auto [oAxisX, oAxisY] = getAxis(box);

		if (!overlapOnAxis(sPos, oPos, sAxisX, sAxisY, box) ||
			!overlapOnAxis(sPos, oPos, oAxisX, oAxisY, box))
		{
			return false;
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

double BoxCollider::findDistance(double startX, double startY, double endX, double endY)
{
	double a = std::abs(startX - endX);
	double b = std::abs(startY - endY);

	return std::sqrt(a * a + b * b);
}

std::pair<glm::vec2, glm::vec2> BoxCollider::getAxis(const BoxCollider* box)
{
	glm::vec2 ox = {1, 0};
	glm::vec2 oy = {0, 1};

	float rot = box->obj->getRot();

	auto rotateX = [](glm::vec2 pos, float rot_) { return pos.x * std::cos(rot_) - pos.y * std::sin(rot_); };
	auto rotateY = [](glm::vec2 pos, float rot_) { return pos.x * std::sin(rot_) + pos.y * std::cos(rot_); };

	ox.x = rotateX(ox, rot);
	ox.y = rotateY(ox, rot);

	oy.x = rotateX(oy, rot);
	oy.y = rotateY(oy, rot);

	return {ox, oy};
}

float BoxCollider::projectOntoAxis(const glm::vec2& pos, const glm::vec2& axisX, const glm::vec2& axisY, const glm::vec2& halfSize)
{
	glm::vec2 corner = pos - axisX * halfSize.x - axisY * halfSize.y;
	return corner.x * axisX.x + corner.y * axisX.y;
}

bool BoxCollider::overlapOnAxis(const glm::vec2& sPos, const glm::vec2& oPos, const glm::vec2& axisX, const glm::vec2& axisY, const BoxCollider* box) const
{
	glm::vec2 sHalfSize = size * 0.5f;
	glm::vec2 oHalfSize = box->size * 0.5f;

	float sProjection = projectOntoAxis(sPos, axisX, axisY, sHalfSize);
	float oProjection = projectOntoAxis(oPos, axisX, axisY, oHalfSize);

	float overlap = std::abs(sProjection - oProjection);
	float combinedHalfSize = sHalfSize.x + oHalfSize.x;

	return overlap < combinedHalfSize;
}
