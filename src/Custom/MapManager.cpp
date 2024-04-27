#include "MapManager.h"

#include "Assets.h"
#include "PolygonCollider.h"
#include "SpriteRenderer.h"
#include "Rigidbody.h"

MapManager::MapManager(GameObject* obj) : Singleton(obj) {}

void MapManager::start()
{
	createMap();
}
void MapManager::createMap()
{
	createGround({0, 0}, {41, 41});

	createWall({-20, 0}, {1, 41});
	createWall({20, 0}, {1, 41});
	createWall({0, -20}, {41, 1});
	createWall({0, 20}, {41, 1});
}
SpriteRenderer* MapManager::createGround(const glm::vec2& pos, const glm::vec2& size)
{
	auto sprite = Assets::load<Sprite>("assets/sprites/square.png");

	auto ground = GameObject::create("Ground", pos)->addComponent<SpriteRenderer>(sprite, size, Color::CYAN.darken(0.8f), -999);
	ground->gameObject()->setTag("Ground");

	return ground;
}
SpriteRenderer* MapManager::createWall(const glm::vec2& pos, const glm::vec2& size)
{
	auto sprite = Assets::load<Sprite>("assets/sprites/square.png");

	auto wall = GameObject::create("Wall", pos)->addComponent<SpriteRenderer>(sprite, size);
	wall->gameObject()->setTag("Wall");
	wall->addComponent<BoxCollider>(size);
	wall->addComponent<Rigidbody>(true);

	return wall;
}
