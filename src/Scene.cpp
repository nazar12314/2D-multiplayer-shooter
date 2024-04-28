#include "Scene.h"

#include "Camera.h"
#include "Rigidbody.h"
#include "CameraFollow.h"
#include "CameraResizer.h"
#include "DOTween.h"
#include "ExampleScenes.h"
#include "Tank.h"
#include "MapManager.h"
#include "Multiplayer.h"
#include "MyMath.h"
#include "PlayerManager.h"


void game()
{
	bool isServer;
	std::cout << "Server (1) or Client (0)? ";
	std::cin >> isServer;

	// Camera
	auto cam = GameObject::create("Camera")->addComponent<Camera>(8, Color::GRAY);
	auto follow = cam->addComponent<CameraFollow>(5);
	cam->addComponent<CameraResizer>(20, 2, 8);

	// Singletons
	GameObject::create("PlayerManager")->addComponent<PlayerManager>();
	GameObject::create("MapManager")->addComponent<MapManager>();
	GameObject::create("Multiplayer")->addComponent<Multiplayer>(isServer);

	follow->setTarget(PlayerManager::instance()->getMainPlayer()->tank()->transform());
}

void Scene::create()
{
	//examples::ExampleScenes::shapeSpawnerNoGravityWithTankScene();

	game();
}
