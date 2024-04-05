#include "Scene.h"

#include "PolygonCollider.h"
#include "Camera.h"
#include "CameraFollow.h"
#include "CameraResizer.h"
#include "Rigidbody.h"
#include "Tank.h"
#include "Assets.h"
#include "DOVirtual.h"
#include "MyMath.h"
#include "ShapeSpawner.h"
#include "TextRenderer.h"
#include "Transform.h"
#include "Wall.h"
#include "TextRenderer.h"

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

	for (int i = 0; i < 50; i++)
	{
		ShapeSpawner::spawnSquare(Math::randomVec2(-10, 10));
		ShapeSpawner::spawnCircle(Math::randomVec2(-10, 10));
	}
}

void textAndTweensScene()
{
	auto cam = Object::create("Camera")->addComponent<Camera>(8);
	auto resizer = cam->addComponent<CameraResizer>(8, 2, 8);

	auto text = Object::create("Text", {0, 1})->addComponent<TextRenderer>("Hello, World!", glm::vec2(5, 5), Color::green.withAlpha(0), 2);
	auto text1 = Object::create("Text", {1, 0})->addComponent<TextRenderer>("Hello, World!", glm::vec2(5, 5), Color::red.withAlpha(0), 2);
	auto text2 = Object::create("Text", {-1, -1})->addComponent<TextRenderer>("Hello, World!", glm::vec2(5, 6), Color::blue.withAlpha(0), 2);

	DOVirtual::delayedCall([text] { text->setColor(text->getColor().withAlpha(1)); }, 1);
	DOVirtual::floatTo(0, 1, 3, [text1](float value) { text1->setColor(Color::red.withAlpha(value)); });
	DOVirtual::colorTo(Color::white.withAlpha(0), Color::blue, 3, [text2](Color value) { text2->setColor(value); });
	DOVirtual::vec2To(text2->transform()->getPos() - glm::vec2(1, 0), text2->transform()->getPos() + glm::vec2(1, 0), 3,
	                  [text2](glm::vec2 value) { text2->transform()->setPos(value); });
}

void Scene::create()
{
	textAndTweensScene();
}
