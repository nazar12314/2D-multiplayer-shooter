#include "MyMath.h"

#include <iostream>

#include "Gizmos.h"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/closest_point.hpp"


float Math::shortestAngle(float currentAngle, float targetAngle)
{
	float angle = fmod(targetAngle - currentAngle, 360.0f);
	if (angle < -180) angle += 360;
	else if (angle > 180) angle -= 360;
	return angle;
}

float Math::rotateTowards(float currentAngle, float targetAngle, float maxStep)
{
	float angleDiff = shortestAngle(currentAngle, targetAngle);
	float step = std::copysign(std::min(std::abs(angleDiff), maxStep), angleDiff);
	return currentAngle + step;
}

float Math::lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}
glm::vec2 Math::lerp(glm::vec2 a, glm::vec2 b, float t)
{
	return a + (b - a) * t;
}

float Math::distance(float startX, float startY, float endX, float endY)
{
	float a = std::abs(startX - endX);
	float b = std::abs(startY - endY);

	return std::sqrt(a * a + b * b);
}
float Math::distanceSquared(glm::vec2 a, glm::vec2 b)
{
	return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}
float Math::cross(glm::vec2 a, glm::vec2 b)
{
	return a.x * b.y - a.y * b.x;
}
float Math::round(float value, int digits)
{
	float factor = (float)std::pow(10, digits);
	return std::round(value * factor) / factor;
}

std::tuple<float, glm::vec2> Math::findMinSeparation(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB)
{
	auto sep = std::numeric_limits<float>::lowest();
	auto sepNorm = glm::vec2(0.0f);
	for (int i = 0; i < verticesA.size(); i++)
	{
		auto v = verticesA[i];
		auto v_next = verticesA[(i + 1) % verticesA.size()];
		auto edge = v_next - v;
		auto norm = normalize(glm::vec2(-edge.y, edge.x));

		auto minSep = std::numeric_limits<float>::max();
		for (const auto& v_ : verticesB)
			minSep = std::min(minSep, dot(v_ - v, norm));

		if (minSep > sep)
		{
			sep = minSep;
			sepNorm = norm;
		}

		if (sep > 0) break;
	}
	return {sep, sepNorm};
}
std::tuple<float, glm::vec2> Math::findMinSeparation(glm::vec2 centerA, float radiusA, const std::vector<glm::vec2>& verticesB)
{
	auto sep = std::numeric_limits<float>::lowest();
	auto sepNorm = glm::vec2(0.0f);
	for (int i = 0; i < verticesB.size(); i++)
	{
		auto v = verticesB[i];
		auto v_next = verticesB[(i + 1) % verticesB.size()];
		auto edge = v_next - v;
		auto norm = normalize(glm::vec2(-edge.y, edge.x));

		auto dir = norm * radiusA;
		auto p1 = centerA - v + dir;
		auto p2 = centerA - v - dir;
		auto minSep = std::min(dot(p1, norm), dot(p2, norm));
		if (minSep > sep)
		{
			sep = minSep;
			sepNorm = -norm;
		}
		if (sep > 0) return {sep, sepNorm};
	}

	int v_index = closestVertexToPoint(centerA, verticesB);
	auto v = verticesB[v_index];
	auto v_prev = verticesB[(v_index - 1 + verticesB.size()) % verticesB.size()];
	auto v_next = verticesB[(v_index + 1) % verticesB.size()];
	if (dot(v_prev - v, centerA - v) > 0 || dot(v_next - v, centerA - v) > 0)
		return {sep, sepNorm};

	auto norm = normalize(centerA - v);
	auto dir = norm * radiusA;
	auto p1 = centerA - v + dir;
	auto p2 = centerA - v - dir;
	auto minSep = std::min(dot(p1, norm), dot(p2, norm));
	if (minSep > sep)
	{
		sep = minSep;
		sepNorm = -norm;
	}

	return {sep, sepNorm};
}
std::tuple<float, glm::vec2> Math::findMinSeparation(glm::vec2 centerA, float radiusA, glm::vec2 centerB, float radiusB)
{
	auto sep = glm::distance(centerA, centerB) - radiusA - radiusB;
	auto sepNorm = centerA != centerB ? normalize(centerB - centerA) : glm::vec2(1, 0);
	return {sep, sepNorm};
}

bool Math::intersects(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB)
{
	auto [sep1, norm1] = findMinSeparation(verticesA, verticesB);
	auto [sep2, norm2] = findMinSeparation(verticesB, verticesA);
	return sep1 <= 0 && sep2 <= 0;
}
bool Math::intersects(glm::vec2 centerA, float radiusA, const std::vector<glm::vec2>& verticesB)
{
	auto [sep, norm] = findMinSeparation(centerA, radiusA, verticesB);
	return sep <= 0;
}
bool Math::intersects(glm::vec2 centerA, float radiusA, glm::vec2 centerB, float radiusB)
{
	return glm::distance(centerA, centerB) - radiusA - radiusB <= 0;
}

std::vector<glm::vec2> Math::findContactPoints(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB)
{
	std::vector<glm::vec2> contactPoints;
	auto minDis = std::numeric_limits<float>::max();
	for (int i = 0; i < verticesA.size(); i++)
	{
		auto p = verticesA[i];
		for (int j = 0; j < verticesB.size(); j++)
		{
			auto v1 = verticesB[j];
			auto v2 = verticesB[(j + 1) % verticesB.size()];

			auto closestPoint = closestPointOnLine(p, v1, v2);
			auto dis = distanceSquared(p, closestPoint);
			if (dis < minDis)
			{
				minDis = dis;
				contactPoints.clear();
				contactPoints.push_back(closestPoint);
			}
			else if (nearlyEqual(dis, minDis, 0.00000001f))
				contactPoints.push_back(closestPoint);
		}
	}

	for (int i = 0; i < verticesB.size(); i++)
	{
		auto p = verticesB[i];
		for (int j = 0; j < verticesA.size(); j++)
		{
			auto v1 = verticesA[j];
			auto v2 = verticesA[(j + 1) % verticesA.size()];

			auto closestPoint = closestPointOnLine(p, v1, v2);
			auto dis = distanceSquared(p, closestPoint);
			if (dis < minDis)
			{
				minDis = dis;
				contactPoints.clear();
				contactPoints.push_back(closestPoint);
			}
			else if (nearlyEqual(dis, minDis))
			{
				if (std::ranges::find(contactPoints, closestPoint) == contactPoints.end())
					contactPoints.push_back(closestPoint);
			}
		}
	}
	int sz = contactPoints.size();
	return contactPoints;
}
std::vector<glm::vec2> Math::findContactPoints(glm::vec2 centerA, float radiusA, const std::vector<glm::vec2>& verticesB)
{
	std::vector<glm::vec2> contactPoints(1);
	auto minDis = std::numeric_limits<float>::max();
	for (int i = 0; i < verticesB.size(); i++)
	{
		auto v1 = verticesB[i];
		auto v2 = verticesB[(i + 1) % verticesB.size()];

		auto closestPoint = closestPointOnLine(centerA, v1, v2);
		auto dis = distanceSquared(centerA, closestPoint);
		if (dis < minDis)
		{
			minDis = dis;
			contactPoints[0] = closestPoint;
		}
	}
	return contactPoints;
}
std::vector<glm::vec2> Math::findContactPoints(glm::vec2 centerA, float radiusA, glm::vec2 centerB, float radiusB)
{
	if (nearlyEqual(centerA, centerB))
		return {centerA + glm::vec2(radiusA, 0)};
	return {centerA + normalize(centerB - centerA) * radiusA};
}

int Math::closestVertexToPoint(glm::vec2 point, const std::vector<glm::vec2>& vertices)
{
	auto index = 0;
	auto minDis = std::numeric_limits<float>::max();
	for (int i = 0; i < vertices.size(); i++)
	{
		auto v = vertices[i];
		auto dis = distanceSquared(point, v);
		if (dis < minDis)
		{
			minDis = dis;
			index = i;
		}
	}
	return index;
}

float Math::randomFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}
float Math::randomInt(int min, int max)
{
	return min + rand() % (max - min);
}
float Math::randomValue()
{
	return randomFloat(0, 1);
}

bool Math::nearlyEqual(float a, float b, float epsilon)
{
	return std::abs(a - b) < epsilon;
}
bool Math::nearlyEqual(glm::vec2 a, glm::vec2 b, float epsilon)
{
	return nearlyEqual(a.x, b.x, epsilon) && nearlyEqual(a.y, b.y, epsilon);
}
bool Math::nearlyZero(float a, float epsilon)
{
	return nearlyEqual(a, 0, epsilon);
}
bool Math::nearlyZero(glm::vec2 a, float epsilon)
{
	return nearlyEqual(a.x, 0) && nearlyEqual(a.y, 0);
}
