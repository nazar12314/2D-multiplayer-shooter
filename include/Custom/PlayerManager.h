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
	std::vector<std::unique_ptr<Player>> players;

	PlayerManager(GameObject* obj) : Singleton(obj) {}

public:
	Player* addPlayer(const net::AddPlayerData& data, bool isMain);
	Player* getPlayer(int id) const;
	Player* getMainPlayer() const;

	void preparePlayerRespawn(Tank* tank);

	friend class GameObject;
	friend class Multiplayer;
};
