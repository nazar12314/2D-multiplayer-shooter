#include "Scene.h"

#include "PolygonCollider.h"
#include "CircleCollider.h"
#include "Camera.h"
#include "CameraFollow.h"
#include "CameraResizer.h"
#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Tank.h"
#include "Assets.h"
#include "glm/ext/scalar_constants.hpp"

void colliderTestScene()
{
	auto cam = Object::create("Camera")->addComponent<Camera>(500);

	auto tex1 = Assets::load<Texture>("sprites/square.png");
	auto tex2 = Assets::load<Texture>("sprites/square.png");

	auto obj1 = Object::create("Square", {10, 10});
	auto obj2 = Object::create("Square", {65, 10});
	obj1->addComponent<SpriteRenderer>(tex1, glm::ivec2(50, 70));
	obj2->addComponent<SpriteRenderer>(tex2, glm::ivec2(50, 50));

	auto box1 = obj1->addComponent<PolygonCollider>(glm::vec2(50, 70));
	auto box2 = obj2->addComponent<PolygonCollider>(glm::vec2(50, 50));
}

void gameScene()
{
	auto cam = Object::create("Camera")->addComponent<Camera>(8);
	auto follow = cam->obj->addComponent<CameraFollow>(5);
	auto resizer = cam->obj->addComponent<CameraResizer>(8, 2, 8);

	auto tank = Object::create("Player")->addComponent<Tank>();
	follow->setTarget(tank->obj);

	for (int i = 0; i < 20; i++)
	{
		auto size = Math::randomFloat(1, 3);
		auto square = Object::create("Square", {Math::randomFloat(-10, 10), Math::randomFloat(-10, 10)}, Math::randomFloat(0, 90));
		square->setTag("Wall");
		square->addComponent<SpriteRenderer>(Assets::load<Texture>("sprites/square.png"), glm::vec2(size, size), 0, Color::randomLight());
		square->addComponent<PolygonCollider>(glm::vec2(size, size));
		auto rb = square->addComponent<Rigidbody>(size * size, 5.0f, 0);
		//rb->setIsStatic(true);
	}

	for (int i = 0; i < 20; i++)
	{
		auto radius = Math::randomFloat(0.5f, 1.5f);
		auto square = Object::create("Circle", {Math::randomFloat(-10, 10), Math::randomFloat(-10, 10)}, Math::randomFloat(0, 90));
		square->setTag("Wall");
		square->addComponent<SpriteRenderer>(Assets::load<Texture>("sprites/circle.png"), glm::vec2(radius * 2, radius * 2), 0, Color::randomLight());
		square->addComponent<CircleCollider>(radius);
		auto rb = square->addComponent<Rigidbody>(glm::pi<float>() * radius * radius, 5.0f, 0);
		//rb->setIsStatic(true);
	}
}

void Scene::create()
{
	gameScene();
}
