#include "Scene.h"

#include "Camera.h"
#include "Rigidbody.h"
#include "CameraFollow.h"
#include "CameraResizer.h"
#include "DOTween.h"
#include "ExampleScenes.h"
#include "Tank.h"
#include "MapManager.h"
#include "MyMath.h"


void game()
{
	auto cam = GameObject::create("Camera")->addComponent<Camera>(8, Color::GRAY);
	auto follow = cam->addComponent<CameraFollow>(5);
	auto resizer = cam->addComponent<CameraResizer>(20, 2, 8);

	auto tank = GameObject::create("Player", Math::randomVec2(-18.0f, 18.0f))->addComponent<Tank>(true);
	follow->setTarget(tank->gameObject());

	GameObject::create("MapManager")->addComponent<MapManager>();
}

void Scene::create()
{
	//examples::ExampleScenes::shapeSpawnerNoGravityWithTankScene();

	game();
}
