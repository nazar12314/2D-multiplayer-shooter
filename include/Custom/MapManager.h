#pragma once

#include "Color.h"
#include "Singleton.h"

class SpriteRenderer;

class MapManager : public Singleton<MapManager>
{
	explicit MapManager(GameObject* obj);

	static SpriteRenderer* createGround(const glm::vec2& pos, const glm::vec2& size);
	static SpriteRenderer* createWall(const glm::vec2& pos, const glm::vec2& size, Color color = Color::WHITE);
	static bool tryCreateInnerWall(const glm::ivec2& pos, const glm::ivec2& size);

public:
	static constexpr int MAP_SIZE = 40;
	static constexpr int WALL_LENGTH = 15;
	static constexpr int WALL_WIDTH = 2;
	inline static int map[MAP_SIZE / WALL_WIDTH][MAP_SIZE / WALL_WIDTH] = {{0}};

	inline static int mapSeed = time(0);

	static void createMap(int seed);

	friend class GameObject;
};
