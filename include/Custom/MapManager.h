#pragma once

#include "Singleton.h"

class SpriteRenderer;

class MapManager : public Singleton<MapManager>
{
	explicit MapManager(GameObject* obj);

	void start() override;
	static void createMap();
	static SpriteRenderer* createGround(const glm::vec2& pos, const glm::vec2& size);
	static SpriteRenderer* createWall(const glm::vec2& pos, const glm::vec2& size);

	friend class GameObject;
};
