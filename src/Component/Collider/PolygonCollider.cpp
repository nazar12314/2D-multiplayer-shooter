#include "PolygonCollider.h"

#include <vector>

#include "MyMath.h"
#include "SpriteRenderer.h"
#include "Assets.h"
#include "CircleCollider.h"
#include "GameObject.h"
#include "Transform.h"

PolygonCollider::PolygonCollider(GameObject* obj, const std::vector<glm::vec2>& vertices, bool isTrigger) : Collider(obj, isTrigger), _vertices(vertices)
{
	if constexpr (DISPLAY_VERTICES_DEBUG)
		for (int i = 0; i < _vertices.size(); i++)
			vertexSpritesTEST.push_back(GameObject::create("pointTEMP")
				->addComponent<SpriteRenderer>(Assets::load<Sprite>("assets/sprites/circle.png"), glm::vec2(0.1f, 0.1f)));
}

std::optional<Collision> PolygonCollider::getCollisionWith(Collider* other)
{
	return other->getCollisionWith(this);
}
std::optional<Collision> PolygonCollider::getCollisionWith(PolygonCollider* other)
{
	auto& vertices = _globalVertices;
	auto& otherVertices = other->_globalVertices;
	auto [sep1, norm1] = Math::findMinSeparation(vertices, otherVertices);
	if (sep1 >= 0) return std::nullopt;

	auto [sep2, norm2] = Math::findMinSeparation(otherVertices, vertices);
	auto collided = sep1 <= 0 && sep2 <= 0;
	if (!collided) return std::nullopt;

	if (sep1 > sep2)
		return Collision(norm1, -sep1, this, other);
	return Collision(norm2, -sep2, other, this);
}
std::optional<Collision> PolygonCollider::getCollisionWith(CircleCollider* other)
{
	auto [sep, norm] = Math::findMinSeparation(other->transform()->getPos(), other->_globalRadius, _globalVertices);
	auto collided = sep <= 0;
	if (!collided) return std::nullopt;

	return Collision(norm, -sep, other, this);
}

std::vector<glm::vec2> PolygonCollider::findContactPoints(const Collider* other) const
{
	return other->findContactPoints(this);
}
std::vector<glm::vec2> PolygonCollider::findContactPoints(const PolygonCollider* other) const
{
	return Math::findContactPoints(_globalVertices, other->_globalVertices);
}
std::vector<glm::vec2> PolygonCollider::findContactPoints(const CircleCollider* other) const
{
	return Math::findContactPoints(other->transform()->getPos(), other->_globalRadius, _globalVertices);
}

bool PolygonCollider::isPointInside(const glm::vec2& point) const
{
	auto vertices = _globalVertices;
	bool c = false;
	for (int i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++)
	{
		if (vertices[i].y >= point.y != (vertices[j].y >= point.y) &&
			point.x <= (vertices[j].x - vertices[i].x) * (point.y - vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x
		)
			c = !c;
	}
	return c;
}
std::optional<Collision> PolygonCollider::getImpactCollision(glm::vec2 center, float radius)
{
	auto [sep, norm] = Math::findMinSeparation(center, radius, _globalVertices);
	auto collided = sep <= 0;
	if (!collided) return std::nullopt;

	auto contactPoints = Math::findContactPoints(center, radius, _globalVertices);
	return Collision(norm, -sep, nullptr, this, contactPoints);
}

void PolygonCollider::recalculate()
{
	_globalVertices.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); i++)
		_globalVertices[i] = transform()->localToGlobalPos(_vertices[i]);

	if constexpr (DISPLAY_VERTICES_DEBUG)
		for (int i = 0; i < _vertices.size(); i++)
			vertexSpritesTEST[i]->transform()->setPos(_globalVertices[i]);
}

void PolygonCollider::onDestroy()
{
	if constexpr (DISPLAY_VERTICES_DEBUG)
		for (auto& point : vertexSpritesTEST)
			destroy(point->gameObject());
}

float PolygonCollider::calculateMass() const
{
	float area = 0;
	for (int n = 0; n < _vertices.size(); ++n)
	{
		auto v1 = _vertices[n];
		auto v2 = _vertices[(n + 1) % _vertices.size()];
		area += std::abs(Math::cross(v2, v1));
	}
	return area / 2;
}
float PolygonCollider::calculateInertia(float mass) const
{
	float sum1 = 0;
	float sum2 = 0;
	for (int n = 0; n < _vertices.size(); ++n)
	{
		auto v1 = _vertices[n];
		auto v2 = _vertices[(n + 1) % _vertices.size()];
		sum1 += Math::cross(v2, v1) * (dot(v1, v1) + dot(v1, v2) + dot(v2, v2));
		sum2 += Math::cross(v2, v1);
	}
	return mass / 6 * sum1 / sum2;
}

void PolygonCollider::setVertices(const std::vector<glm::vec2>& vertices)
{
	_vertices = vertices;
	recalculate();
}

BoxCollider::BoxCollider(GameObject* obj, glm::vec2 size, bool isTrigger) : PolygonCollider(obj, calculateBoxVertices(size), isTrigger), _size(size) {}

std::vector<glm::vec2> BoxCollider::calculateBoxVertices(glm::vec2 size)
{
	return {
		{-size.x / 2, -size.y / 2}, // bottom left
		{-size.x / 2, +size.y / 2}, // top left
		{+size.x / 2, +size.y / 2}, // top right
		{+size.x / 2, -size.y / 2} // bottom right
	};
}

glm::vec2 BoxCollider::size() const { return _size; }
void BoxCollider::setSize(glm::vec2 size)
{
	_size = size;
	setVertices(calculateBoxVertices(size));
}
