#pragma once

#include "Color.h"
#include "Component.h"
#include "Singleton.h"

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
};

class PlayerManager : public Singleton<PlayerManager>
{
	std::vector<std::unique_ptr<Player>> players;

	PlayerManager(GameObject* obj) : Singleton(obj) {}

public:
	Player* addPlayer(const net::AddPlayerData& data, bool isMain);
	Player* getPlayer(int id) const;
	Player* getMainPlayer() const;

	friend class GameObject;
	friend class Multiplayer;
};
