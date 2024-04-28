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
#include "PlayerManager.h"


void game()
{
	// Camera
	auto cam = GameObject::create("Camera")->addComponent<Camera>(8, Color::GRAY);
	auto follow = cam->addComponent<CameraFollow>(5);
	cam->addComponent<CameraResizer>(20, 2, 8);

	// Singletons
	GameObject::create("PlayerManager")->addComponent<PlayerManager>();
	GameObject::create("MapManager")->addComponent<MapManager>();

	// Players
	auto player1Main = PlayerManager::instance()->addPlayer("Player 1", true);
	auto player2 = PlayerManager::instance()->addPlayer("Player 2", false);

	follow->setTarget(player1Main->tank()->transform());
}

void Scene::create()
{
	//examples::ExampleScenes::shapeSpawnerNoGravityWithTankScene();

	game();
}
