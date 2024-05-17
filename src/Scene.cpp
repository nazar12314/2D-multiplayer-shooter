#include "Scene.h"

#include "Button.h"
#include "Camera.h"
#include "Rigidbody.h"
#include "CameraFollow.h"
#include "CameraResizer.h"
#include "InputField.h"
#include "Tweener.h"
#include "MapManager.h"
#include "Multiplayer.h"
#include "MyMath.h"
#include "PlayerManager.h"
#include "ScoreDisplayer.h"

void game();

void connectionMenu()
{
	GameObject::create("Camera")->addComponent<Camera>(30, Color::GRAY);

	auto hostPortField = GameObject::create("InputField", {-8, 0})->addComponent<InputField>("1234", glm::vec2(10, 1), Color::BLACK);

	auto hostButton = GameObject::create("Button", {-8, -2})->addComponent<Button>("Host", glm::vec2(10, 1), Color::BLACK);
	hostButton->setBgColor(Color::CYAN);
	hostButton->onClick += [hostPortField]
	{
		Multiplayer::isServer = true;
		Multiplayer::serverIP = "127.0.0.1";
		Multiplayer::serverPort = hostPortField->getText();

		GameObject::destroyAll();
		game();
	};

	auto connIpField = GameObject::create("InputField", {8, 2})->addComponent<InputField>("127.0.0.1", glm::vec2(10, 1), Color::BLACK);
	auto connPortField = GameObject::create("InputField", {8, 0})->addComponent<InputField>("1234", glm::vec2(10, 1), Color::BLACK);

	auto connButton = GameObject::create("Button", {8, -2})->addComponent<Button>("Connect", glm::vec2(10, 1), Color::BLACK);
	connButton->setBgColor(Color::CYAN);
	connButton->onClick += [connIpField, connPortField]
	{
		Multiplayer::isServer = false;
		Multiplayer::serverIP = connIpField->getText();
		Multiplayer::serverPort = connPortField->getText();

		GameObject::destroyAll();
		game();
	};
}

void game()
{
	// Camera
	auto cam = GameObject::create("Camera")->addComponent<Camera>(50, Color::GRAY);
	cam->addComponent<CameraFollow>(5);
	cam->addComponent<CameraResizer>(2, 8);

	// Singletons
	GameObject::create("PlayerManager")->addComponent<PlayerManager>();
	GameObject::create("MapManager")->addComponent<MapManager>();
	GameObject::create("ScoreDisplayer")->addComponent<ScoreDisplayer>();

	GameObject::create("Multiplayer")->addComponent<Multiplayer>();
}

void Scene::create()
{
	connectionMenu();
}
