#include "Scene.h"

#include <iostream>
#include <ostream>

#include "PolygonCollider.h"
#include "Camera.h"
#include "CameraMover.h"
#include "CircleCollider.h"
#include "Graphical.h"
#include "Texture.h"

void Scene::init()
{
	auto cam = Object::create("Camera")->addComponent<Camera>(500);
	auto mover = cam->obj->addComponent<CameraMover>();
	mover->speed = 200;

	auto tex1 = new Texture("../sprites/square.png");
	auto tex2 = new Texture("../sprites/square.png");

	auto obj1 = Object::create("Square", {10, 10});
    auto obj2 = Object::create("Square", {65, 10});
//	auto obj2 = Object::create("Circle", {100, 10});

    obj2->setRot(45);

	obj1->addComponent<Graphical>(tex1, glm::ivec2(50, 70));
	obj2->addComponent<Graphical>(tex2, glm::ivec2(50, 50));

	auto box1 = obj1->addComponent<PolygonCollider>(glm::vec2(50, 70));
    auto box2 = obj2->addComponent<PolygonCollider>(glm::vec2(50, 50));

	std::cout << "collides: " << box1->collidesWith(box2) << std::endl;
}
