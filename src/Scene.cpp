#include "Scene.h"

#include <iostream>
#include <ostream>

#include "PolygonCollider.h"
#include "Camera.h"
#include "CameraFollow.h"
#include "CircleCollider.h"
#include "PlayerMovement.h"
#include "Sprite.h"
#include "Texture.h"

void colliderTestScene()
{
	auto cam = Object::create("Camera")->addComponent<Camera>(500);

	auto tex1 = new Texture("sprites/square.png");
	auto tex2 = new Texture("sprites/square.png");

	auto obj1 = Object::create("Square", {10, 10});
	auto obj2 = Object::create("Square", {65, 10});
	obj1->addComponent<Sprite>(tex1, glm::ivec2(50, 70));
	obj2->addComponent<Sprite>(tex2, glm::ivec2(50, 50));

	auto box1 = obj1->addComponent<PolygonCollider>(glm::vec2(50, 70));
	auto box2 = obj2->addComponent<PolygonCollider>(glm::vec2(50, 50));
}

void gameScene()
{
	auto squareTex = new Texture("sprites/square.png");

	auto cam = Object::create("Camera")->addComponent<Camera>(8);
	auto follow = cam->obj->addComponent<CameraFollow>(5);

	auto player = Object::create("Player");
	player->addComponent<Sprite>(squareTex, glm::ivec2(1, 1));
	auto playerMov = player->addComponent<PlayerMovement>(3);
	player->addComponent<PolygonCollider>(glm::vec2(1, 1));

	follow->setTarget(player);

	auto wall1 = Object::create("Wall", {5, 1})->addComponent<Sprite>(squareTex, glm::ivec2(5, 1));
	auto wall1Col = wall1->obj->addComponent<PolygonCollider>(glm::vec2(5, 1));

	playerMov->col = wall1Col;
}

void Scene::init()
{
	gameScene();
}
