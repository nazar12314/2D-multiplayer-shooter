#pragma once

#include "Color.h"
#include "Component.h"
#include "Singleton.h"

class TextRenderer;

namespace net
{
	struct AddPlayerData;
}

class Tank;

struct Player
{
	int id;
	bool isMain = false;
	std::string name;
	Color color;
	Tank* tank;

	Player(int id, bool isMain, const std::string& name, const Color& color, Tank* tank) : id(id), isMain(isMain), name(name), color(color), tank(tank) {}
};

class PlayerManager : public Singleton<PlayerManager>
{
	static constexpr float MIN_DIS_TO_OTHER = 10.0f;

	std::vector<std::unique_ptr<Player>> players;

	PlayerManager(GameObject* obj) : Singleton(obj) {}

public:
	Player* addPlayer(const net::AddPlayerData& data, bool isMain);
	Player* getPlayer(int id) const;
	Player* getMainPlayer() const;

	void preparePlayerRespawn(const Tank* tank) const;
	glm::vec2 getSpawnPos(Player* player = nullptr) const;

	friend class GameObject;
	friend class Multiplayer;
};
