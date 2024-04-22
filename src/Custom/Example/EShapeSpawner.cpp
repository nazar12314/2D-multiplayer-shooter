#include "EShapeSpawner.h"

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

namespace examples
{
	EShapeSpawner::EShapeSpawner(GameObject* obj, bool enableGravity, bool spawnOnClick) : Component(obj), _enableGravity(enableGravity), _spawnOnClick(spawnOnClick) { }

	void EShapeSpawner::update()
	{
		if (_spawnOnClick)
		{
			if (_spawnTimer > 0)
			{
				_spawnTimer -= Time::deltaTime();
				return;
			}
			_spawnTimer = _spawnDelay;

			auto spawnPos = Camera::getMain()->screenToWorldPoint(Input::mousePos());
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

	GameObjectSPtr EShapeSpawner::spawnSquare(glm::vec2 spawnPos, bool enableGravity)
	{
		auto size = Math::randomFloat(0.5f, 1);
		auto obj = GameObject::create("Square", spawnPos);
		obj->transform()->setScale(glm::vec2(size, size));
		obj->setTag("Wall");
		obj->addComponent<SpriteRenderer>(Assets::load<Sprite>("assets/sprites/square.png"), glm::vec2(1, 1), Color::randomLight());
		obj->addComponent<BoxCollider>(glm::vec2(1, 1));
		auto rb = obj->addComponent<Rigidbody>(enableGravity ? 0 : 5, enableGravity ? 0 : 5);
		rb->setRestitution(0.5f);
		if (enableGravity) rb->setGravity(10);

		_spawnedShapes.push_back(obj);
		return obj;
	}
	GameObjectSPtr EShapeSpawner::spawnCircle(glm::vec2 spawnPos, bool enableGravity)
	{
		auto size = Math::randomFloat(0.5f, 1.0f);
		auto obj = GameObject::create("Circle", spawnPos);
		obj->transform()->setScale(glm::vec2(size, size));
		obj->setTag("Wall");
		obj->addComponent<SpriteRenderer>(Assets::load<Sprite>("assets/sprites/circle.png"), glm::vec2(1, 1), Color::randomLight());
		obj->addComponent<CircleCollider>(0.5f);
		auto rb = obj->addComponent<Rigidbody>(enableGravity ? 0 : 5, enableGravity ? 0 : 5);
		rb->setRestitution(0.5f);
		if (enableGravity) rb->setGravity(10);

		_spawnedShapes.push_back(obj);
		return obj;
	}
}
