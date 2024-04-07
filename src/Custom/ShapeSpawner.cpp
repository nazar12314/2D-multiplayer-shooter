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
#include "Transform.h"
#include "glm/ext/scalar_constants.hpp"

ShapeSpawner::ShapeSpawner(GameObject* obj, bool enableGravity, bool spawnOnClick) : Component(obj), _enableGravity(enableGravity), _spawnOnClick(spawnOnClick) { }

void ShapeSpawner::update()
{
	if (_spawnOnClick)
	{
		if (_spawnTimer > 0)
		{
			_spawnTimer -= Time::deltaTime;
			return;
		}
		_spawnTimer = _spawnDelay;

		auto spawnPos = Camera::getMain()->screenToWorldPoint(Input::mousePos);
		if (Input::isMouseButtonDown(SDL_BUTTON_LEFT))
			spawnSquare(spawnPos, _enableGravity);
		if (Input::isMouseButtonDown(SDL_BUTTON_RIGHT))
			spawnCircle(spawnPos, _enableGravity);
	}

	if (_enableGravity)
	{
		_spawnedShapes.apply_changes();
		for (int i = 0; i < _spawnedShapes.size(); i++)
		{
			auto shape = _spawnedShapes[i];
			if (shape->transform()->getPos().y >= -30) continue;

			destroy(shape.get());
			_spawnedShapes.erase_delayed(shape);
		}
	}
}

GameObjectSPtr ShapeSpawner::spawnSquare(glm::vec2 spawnPos, bool enableGravity)
{
	auto size = Math::randomFloat(0.5f, 1);
	auto obj = GameObject::create("Square", spawnPos);
	obj->setTag("Wall");
	obj->addComponent<SpriteRenderer>(Assets::load<Sprite>("assets/sprites/square.png"), glm::vec2(size, size), Color::randomLight());
	obj->addComponent<PolygonCollider>(glm::vec2(size, size));
	auto rb = obj->addComponent<Rigidbody>(enableGravity ? 0 : 5, enableGravity ? 0 : 5);
	rb->setRestitution(0.5f);
	if (enableGravity) rb->setGravity(10);

	_spawnedShapes.push_back(obj);
	return obj;
}
GameObjectSPtr ShapeSpawner::spawnCircle(glm::vec2 spawnPos, bool enableGravity)
{
	auto radius = Math::randomFloat(0.25f, 0.5f);
	auto obj = GameObject::create("Circle", spawnPos);
	obj->setTag("Wall");
	obj->addComponent<SpriteRenderer>(Assets::load<Sprite>("assets/sprites/circle.png"), glm::vec2(radius * 2, radius * 2), Color::randomLight());
	obj->addComponent<CircleCollider>(radius);
	auto rb = obj->addComponent<Rigidbody>(enableGravity ? 0 : 5, enableGravity ? 0 : 5);
	rb->setRestitution(0.5f);
	if (enableGravity) rb->setGravity(10);

	_spawnedShapes.push_back(obj);
	return obj;
}
