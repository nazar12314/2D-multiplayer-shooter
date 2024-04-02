#include "PolygonCollider.h"

#include <vector>

#include "Math.h"
#include "SpriteRenderer.h"
#include "Assets.h"
#include "CircleCollider.h"
#include "Object.h"
#include "glm/detail/func_trigonometric.inl"

PolygonCollider::PolygonCollider(Object* obj, glm::vec2 size, bool isTrigger): Collider(obj, isTrigger), _size(size)
{
	updateVertices();

	if constexpr (DISPLAY_VERTICES_DEBUG)
		for (int i = 0; i < _vertices.size(); i++)
			vertexSpritesTEST.push_back(Object::create("pointTEMP")->addComponent<SpriteRenderer>(Assets::load<Texture>("sprites/circle.png"), glm::vec2(0.1f, 0.1f)));
}
void PolygonCollider::updateVertices()
{
	// TODO: generalize for any number of vertices
	_vertices.resize(4);
	_vertices[0] = {-_size.x / 2, -_size.y / 2}; // bottom left
	_vertices[1] = {-_size.x / 2, +_size.y / 2}; // top left
	_vertices[2] = {+_size.x / 2, +_size.y / 2}; // top right
	_vertices[3] = {+_size.x / 2, -_size.y / 2}; // bottom right;
}

Collision PolygonCollider::getCollisionWith(Collider* other)
{
	return other->getCollisionWith(this);
}
Collision PolygonCollider::getCollisionWith(PolygonCollider* other)
{
	auto vertices = calculateGlobalVertices();
	auto otherVertices = other->calculateGlobalVertices();
	auto [sep1, norm1] = Math::findMinSeparation(vertices, otherVertices);
	auto [sep2, norm2] = Math::findMinSeparation(otherVertices, vertices);
	auto contactPoints = Math::findContactPoints(vertices, otherVertices);

	auto collided = sep1 < 0 && sep2 < 0;
	if (sep1 > sep2)
		return Collision(collided, norm1, -sep1, contactPoints, this, other);
	return Collision(collided, norm2, -sep2, contactPoints, other, this);
}
Collision PolygonCollider::getCollisionWith(CircleCollider* other)
{
	auto vertices = calculateGlobalVertices();
	auto [sep, norm] = Math::findMinSeparation(other->obj->pos(), other->radius(), vertices);
	auto contactPoints = Math::findContactPoints(other->obj->pos(), other->radius(), vertices);

	auto collided = sep < 0;
	return Collision(collided, norm, -sep, contactPoints, other, this);
}

std::vector<glm::vec2> PolygonCollider::calculateGlobalVertices() const
{
	std::vector<glm::vec2> vertices(_vertices.size());
	for (int i = 0; i < _vertices.size(); i++)
		vertices[i] = obj->localToGlobalPos(_vertices[i]);

	if constexpr (DISPLAY_VERTICES_DEBUG)
		for (int i = 0; i < _vertices.size(); i++)
			vertexSpritesTEST[i]->obj->setPos(vertices[i]);
	return vertices;
}

void PolygonCollider::onDestroy()
{
	if constexpr (DISPLAY_VERTICES_DEBUG)
		for (auto& point : vertexSpritesTEST)
			Object::destroy(point->obj);
}
float PolygonCollider::calculateInertia(float mass) const
{
	float sum1 = 0;
	float sum2 = 0;
	for (int n = 0; n < _vertices.size(); ++n)
	{
		sum1 += Math::cross(_vertices[n + 1], _vertices[n]) * 
			(dot(_vertices[n + 1], _vertices[n + 1]) + dot(_vertices[n + 1], _vertices[n]) + dot(_vertices[n], _vertices[n]));
		sum2 += Math::cross(_vertices[n + 1], _vertices[n]);
	}
	return (mass / 6 * sum1 / sum2);
}

void PolygonCollider::size(glm::vec2 size) { _size = size; }
void PolygonCollider::setSize(glm::vec2 size)
{
	_size = size;
	updateVertices();
}
