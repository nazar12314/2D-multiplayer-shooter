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
#include "ShapeSpawner.h"
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

	Object::create("ShapeSpawner")->addComponent<ShapeSpawner>();
}

void Scene::create()
{
	gameScene();
}
