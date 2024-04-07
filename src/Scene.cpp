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
	auto cam = GameObject::create("Camera")->addComponent<Camera>(8);
	auto follow = cam->addComponent<CameraFollow>(5);
	auto resizer = cam->addComponent<CameraResizer>(8, 2, 8);

	auto tank = GameObject::create("Player")->addComponent<Tank>();
	follow->setTarget(tank->gameObject());

	GameObject::create("ShapeSpawner")->addComponent<ShapeSpawner>();
}

void lotsOfShapesWithGravityScene()
{
	auto cam = GameObject::create("Camera")->addComponent<Camera>(8);
	auto resizer = cam->addComponent<CameraResizer>(20, 2, 8);

	auto shapeSpawner = GameObject::create("ShapeSpawner")->addComponent<ShapeSpawner>(true);

	GameObject::create("Wall", glm::vec2((-5 + 0.25f) / 1.414f, -4), 45)->addComponent<Wall>(glm::vec2(10, 0.5f));
	GameObject::create("Wall", glm::vec2((5 - 0.25f) / 1.414f, -4), -45)->addComponent<Wall>(glm::vec2(10, 0.5f));

	for (int i = 0; i < 15; i++)
	{
		shapeSpawner->spawnSquare(glm::vec2(Math::randomFloat(-4, 4), Math::randomFloat(0, 5)));
		shapeSpawner->spawnCircle(glm::vec2(Math::randomFloat(-4, 4), Math::randomFloat(0, 5)));
	}
}

void shapeSpawnerWithGravity()
{
	auto cam = GameObject::create("Camera")->addComponent<Camera>(8);
	auto resizer = cam->addComponent<CameraResizer>(20, 2, 15, true);

	auto shapeSpawner = GameObject::create("ShapeSpawner")->addComponent<ShapeSpawner>(true);

	GameObject::create("Wall", glm::vec2(0, -5), 0)->addComponent<Wall>(glm::vec2(15, 0.5f));
	GameObject::create("Wall", glm::vec2(3, 1), 15)->addComponent<Wall>(glm::vec2(5, 0.25f));
	GameObject::create("Wall", glm::vec2(-3, -1), -15)->addComponent<Wall>(glm::vec2(5, 0.25f));
}

void impactBulletsTankScene()
{
	auto cam = GameObject::create("Camera")->addComponent<Camera>(8);
	auto follow = cam->addComponent<CameraFollow>(5);
	auto resizer = cam->addComponent<CameraResizer>(8, 2, 8);

	auto tank = GameObject::create("Player")->addComponent<Tank>(true);
	follow->setTarget(tank->gameObject());

	auto shapeSpawner = GameObject::create("ShapeSpawner")->addComponent<ShapeSpawner>(false, false);

	for (int i = 0; i < 50; i++)
	{
		shapeSpawner->spawnSquare(Math::randomVec2(-10, 10));
		shapeSpawner->spawnCircle(Math::randomVec2(-10, 10));
	}
}

void textAndTweensScene()
{
	auto cam = GameObject::create("Camera")->addComponent<Camera>(8);
	auto resizer = cam->addComponent<CameraResizer>(8, 2, 8);

	auto text = GameObject::create("Text", {0, 1})->addComponent<TextRenderer>("Hello, World!", glm::vec2(5, 5), Color::green.withAlpha(0), 2);
	auto text1 = GameObject::create("Text", {1, 0})->addComponent<TextRenderer>("Hello, World!", glm::vec2(5, 5), Color::red.withAlpha(0), 2);
	auto text2 = GameObject::create("Text", {-1, -1})->addComponent<TextRenderer>("Hello, World!", glm::vec2(5, 6), Color::blue.withAlpha(0), 2);

	DOVirtual::delayedCall([text] { text->setColor(text->getColor().withAlpha(1)); }, 1);
	DOVirtual::floatTo(0, 1, 3, [text1](float value) { text1->setColor(Color::red.withAlpha(value)); });
	DOVirtual::colorTo(Color::white.withAlpha(0), Color::blue, 3, [text2](Color value) { text2->setColor(value); });
	auto tween = DOVirtual::vec2To(text2->transform()->getPos() - glm::vec2(1, 0), text2->transform()->getPos() + glm::vec2(1, 0), 3,
	                               [text2](glm::vec2 value) { text2->transform()->setPos(value); });

	//DOVirtual::delayedCall([tween] { tween->kill(); }, 1);
}

void Scene::create()
{
	impactBulletsTankScene();
}
