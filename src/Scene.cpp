#include "Scene.h"

#include "PolygonCollider.h"
#include "Camera.h"
#include "CameraFollow.h"
#include "CameraResizer.h"
#include "Rigidbody.h"
#include "Tank.h"
#include "Assets.h"
#include "MyMath.h"
#include "ShapeSpawner.h"
#include "Transform.h"
#include "Wall.h"

void shapeSpawnerNoGravityWithTankScene()
{
	auto cam = Object::create("Camera")->addComponent<Camera>(8);
	auto follow = cam->addComponent<CameraFollow>(5);
	auto resizer = cam->addComponent<CameraResizer>(8, 2, 8);

	auto tank = Object::create("Player")->addComponent<Tank>();
	follow->setTarget(tank->obj());

	Object::create("ShapeSpawner")->addComponent<ShapeSpawner>();
}

void lotsOfShapesWithGravityScene()
{
	auto cam = Object::create("Camera")->addComponent<Camera>(8);
	auto resizer = cam->addComponent<CameraResizer>(20, 2, 8);

	auto shapeSpawner = Object::create("ShapeSpawner")->addComponent<ShapeSpawner>(true);

	Object::create("Wall", glm::vec2((-5 + 0.25f) / 1.414f, -4), 45)->addComponent<Wall>(glm::vec2(10, 0.5f));
	Object::create("Wall", glm::vec2((5 - 0.25f) / 1.414f, -4), -45)->addComponent<Wall>(glm::vec2(10, 0.5f));

	for (int i = 0; i < 15; i++)
	{
		ShapeSpawner::spawnSquare(glm::vec2(Math::randomFloat(-4, 4), Math::randomFloat(0, 5)));
		ShapeSpawner::spawnCircle(glm::vec2(Math::randomFloat(-4, 4), Math::randomFloat(0, 5)));
	}
}

void shapeSpawnerWithGravity()
{
	auto cam = Object::create("Camera")->addComponent<Camera>(8);
	auto resizer = cam->addComponent<CameraResizer>(20, 2, 15, true);

	auto shapeSpawner = Object::create("ShapeSpawner")->addComponent<ShapeSpawner>(true);

	Object::create("Wall", glm::vec2(0, -5), 0)->addComponent<Wall>(glm::vec2(15, 0.5f));
	Object::create("Wall", glm::vec2(3, 1), 15)->addComponent<Wall>(glm::vec2(5, 0.25f));
	Object::create("Wall", glm::vec2(-3, -1), -15)->addComponent<Wall>(glm::vec2(5, 0.25f));
}

void impactBulletsTankScene()
{
	auto cam = Object::create("Camera")->addComponent<Camera>(8);
	auto follow = cam->addComponent<CameraFollow>(5);
	auto resizer = cam->addComponent<CameraResizer>(8, 2, 8);

	auto tank = Object::create("Player")->addComponent<Tank>(true, true);
	follow->setTarget(tank->obj());

	for (int i = 0; i < 1; i++)
	{
		ShapeSpawner::spawnSquare(Math::randomVec2(-10, 10));
		ShapeSpawner::spawnCircle(Math::randomVec2(-10, 10));
	}
}

void Scene::create()
{
	impactBulletsTankScene();
}
