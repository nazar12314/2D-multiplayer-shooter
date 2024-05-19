// ReSharper disable CppTooWideScope
#include "MapManager.h"

#include "Assets.h"
#include "Multiplayer.h"
#include "MyMath.h"
#include "PolygonCollider.h"
#include "SpriteRenderer.h"
#include "Rigidbody.h"

MapManager::MapManager(GameObject* obj) : Singleton(obj) {}

void MapManager::createMap(int seed)
{
	mapSeed = seed;
	Math::setRandomSeed(seed);

	createGround({0, 0}, {MAP_SIZE + 2, MAP_SIZE + 2});

	// Outer walls
	createWall({-(MAP_SIZE + WALL_WIDTH + 2) / 2.0f, 0}, {WALL_WIDTH, MAP_SIZE + WALL_WIDTH * 2 + 2});
	createWall({(MAP_SIZE + WALL_WIDTH + 2) / 2.0f, 0}, {WALL_WIDTH, MAP_SIZE + WALL_WIDTH * 2 + 2});
	createWall({0, -(MAP_SIZE + WALL_WIDTH + 2) / 2.0f}, {MAP_SIZE + WALL_WIDTH * 2 + 2, WALL_WIDTH});
	createWall({0, (MAP_SIZE + WALL_WIDTH + 2) / 2.0f}, {MAP_SIZE + WALL_WIDTH * 2 + 2, WALL_WIDTH});

	// Inner walls
	for (int i = 0; i < 5; ++i)
	{
		int counter = 0;
		glm::ivec2 randPos;
		do
		{
			int x = Math::randomInt((-MAP_SIZE / 2 + WALL_LENGTH / 2) / WALL_WIDTH, (MAP_SIZE / 2 - WALL_LENGTH / 2) / WALL_WIDTH);
			int y = Math::randomInt((-MAP_SIZE / 2 + 3) / WALL_WIDTH, (MAP_SIZE / 2 - 3) / WALL_WIDTH);
			randPos = {x, y};
		}
		while (counter++ < 100 && !tryCreateInnerWall(randPos, {WALL_LENGTH / WALL_WIDTH, 1}));
	}

	for (int x = 0; x < MAP_SIZE / WALL_WIDTH; ++x)
		for (int y = 0; y < MAP_SIZE / WALL_WIDTH; ++y)
			if (map[x][y] == 2)
				map[x][y] = 0;

	for (int i = 0; i < 5; ++i)
	{
		int counter = 0;
		glm::ivec2 randPos;
		do
		{
			int x = Math::randomInt((-MAP_SIZE / 2 + 3) / WALL_WIDTH, (MAP_SIZE / 2 - 3) / WALL_WIDTH);
			int y = Math::randomInt((-MAP_SIZE / 2 + WALL_LENGTH / 2) / WALL_WIDTH, (MAP_SIZE / 2 - WALL_LENGTH / 2) / WALL_WIDTH);
			randPos = {x, y};
		}
		while (counter++ < 100 && !tryCreateInnerWall(randPos, {1, WALL_LENGTH / WALL_WIDTH - (Math::randomValue() > 0.5f ? 2 : 0)}));
	}
}
SpriteRenderer* MapManager::createGround(const glm::vec2& pos, const glm::vec2& size)
{
	auto sprite = Assets::load<Sprite>("assets/sprites/square.png");

	auto ground = GameObject::create("Ground", pos)->addComponent<SpriteRenderer>(sprite, size, Color::CYAN.darken(0.8f), -999);
	ground->gameObject()->setTag("Ground");

	return ground;
}
SpriteRenderer* MapManager::createWall(const glm::vec2& pos, const glm::vec2& size, Color color)
{
	auto sprite = Assets::load<Sprite>("assets/sprites/square.png");

	auto wall = GameObject::create("Wall", pos)->addComponent<SpriteRenderer>(sprite, size, color);
	wall->gameObject()->setTag("Wall");
	wall->addComponent<BoxCollider>(size);
	wall->addComponent<Rigidbody>(true);

	return wall;
}

bool MapManager::tryCreateInnerWall(const glm::ivec2& pos, const glm::ivec2& size)
{
	auto ok = true;
	for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			if (map[pos.x + i + MAP_SIZE / 2 / WALL_WIDTH - size.x / 2][pos.y + j + MAP_SIZE / 2 / WALL_WIDTH - size.y / 2] == 0) continue;
			ok = false;
			break;
		}
	}
	if (!ok) return false;

	for (int i = 0; i < size.x; i++)
		for (int j = 0; j < size.y; j++)
			map[pos.x + i + MAP_SIZE / 2 / WALL_WIDTH - size.x / 2][pos.y + j + MAP_SIZE / 2 / WALL_WIDTH - size.y / 2] = 1;

	constexpr int wideness = 4 / WALL_WIDTH;
	for (int i = 0; i < size.x + wideness * 2; i++)
	{
		for (int j = 0; j < size.y + wideness * 2; j++)
		{
			int x = pos.x + i + MAP_SIZE / 2 / WALL_WIDTH - size.x / 2 - wideness;
			int y = pos.y + j + MAP_SIZE / 2 / WALL_WIDTH - size.y / 2 - wideness;
			if (x < 0 || y < 0 || x >= MAP_SIZE / WALL_WIDTH || y >= MAP_SIZE / WALL_WIDTH || map[x][y] != 0) continue;
			map[x][y] = 2;
		}
	}

	createWall(pos * WALL_WIDTH, size * WALL_WIDTH, Color::randomLight().desaturate(0.2f));
	return true;
}
