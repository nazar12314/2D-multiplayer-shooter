#include "Scene.h"

#include <iostream>
#include <ostream>

#include "BoxCollider.h"
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

	auto tex1 = new Texture("sprites/square.png");
	auto tex2 = new Texture("sprites/circle.png");

	auto obj1 = Object::create("Square", {10, 10});
	auto obj2 = Object::create("Circle", {100, 10});
	obj1->addComponent<Graphical>(tex1, glm::ivec2(100, 200));
	obj2->addComponent<Graphical>(tex2, glm::ivec2(100, 100));

	auto box1 = obj1->addComponent<BoxCollider>();
	box1->size = {100, 200};
	obj1->setRot(10);

	auto circle1 = obj2->addComponent<CircleCollider>();
	circle1->radius = 50;
	std::cout << "collides: " << box1->collidesWith(circle1) << std::endl;
}
