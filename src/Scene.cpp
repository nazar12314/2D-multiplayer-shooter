#include "Scene.h"

#include "Camera.h"
#include "Rigidbody.h"
#include "CameraFollow.h"
#include "CameraResizer.h"
#include "Tweener.h"
#include "MapManager.h"
#include "Multiplayer.h"
#include "MyMath.h"
#include "PlayerManager.h"
#include "ScoreDisplayer.h"

void game()
{
	// Camera
	auto cam = GameObject::create("Camera")->addComponent<Camera>(50, Color::GRAY);
	auto follow = cam->addComponent<CameraFollow>(5);
	cam->addComponent<CameraResizer>(2, 8);

	// Singletons
	GameObject::create("PlayerManager")->addComponent<PlayerManager>();
	GameObject::create("MapManager")->addComponent<MapManager>();
	GameObject::create("ScoreDisplayer")->addComponent<ScoreDisplayer>();

	GameObject::create("Multiplayer")->addComponent<Multiplayer>(Multiplayer::isServerCONFIG);
}

void Scene::create()
{
	game();
}
