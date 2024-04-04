#include "ShapeSpawner.h"

#include "Assets.h"
#include "Camera.h"
#include "Color.h"
#include "Input.h"
#include "PolygonCollider.h"
#include "CircleCollider.h"
#include "MyMath.h"
#include "MyTime.h"
#include "SpriteRenderer.h"
#include "Rigidbody.h"
#include "glm/ext/scalar_constants.hpp"

void ShapeSpawner::spawnSquare(glm::vec2 spawnPos) const
{
	auto size = Math::randomFloat(0.5f, 1);
	auto obj = Object::create("Square", spawnPos);
	obj->setTag("Wall");
	obj->addComponent<SpriteRenderer>(Assets::load<Texture>("sprites/square.png"), glm::vec2(size, size), 0, Color::randomLight());
	obj->addComponent<PolygonCollider>(glm::vec2(size, size));
	auto rb = obj->addComponent<Rigidbody>(enableGravity ? 0 : 5.0f, 0);
	rb->setRestitution(0.5f);
	if (enableGravity) rb->setGravity(20);
}
void ShapeSpawner::spawnCircle(glm::vec2 spawnPos) const
{
	auto radius = Math::randomFloat(0.25f, 0.5f);
	auto obj = Object::create("Circle", spawnPos);
	obj->setTag("Wall");
	obj->addComponent<SpriteRenderer>(Assets::load<Texture>("sprites/circle.png"), glm::vec2(radius * 2, radius * 2), 0, Color::randomLight());
	obj->addComponent<CircleCollider>(radius);
	auto rb = obj->addComponent<Rigidbody>(enableGravity ? 0 : 5.0f, 0);
	rb->setRestitution(0.5f);
	if (enableGravity) rb->setGravity(20);
}

void ShapeSpawner::update()
{
	if (_spawnTimer > 0)
	{
		_spawnTimer -= Time::deltaTime;
		return;
	}
	_spawnTimer = spawnDelay;

	auto spawnPos = Camera::getMain()->screenToWorldPoint(Input::mousePos);
	if (Input::isMouseButtonDown(SDL_BUTTON_LEFT))
		spawnSquare(spawnPos);
	if (Input::isMouseButtonDown(SDL_BUTTON_RIGHT))
		spawnCircle(spawnPos);
}
