#pragma once

#include "Component.h"
#include "Singleton.h"

class Tank;

class Player
{
	int _id;
	bool _isMain = false;
	std::string _name;
	Tank* _tank;

public:
	Player(int id, bool isMain, const std::string& name, Tank* tank);

	int id() const;
	bool isMain() const;
	std::string name() const;
	Tank* tank() const;

	friend class PlayerManager;
};

class PlayerManager : public Singleton<PlayerManager>
{
	std::vector<std::unique_ptr<Player>> players;

	PlayerManager(GameObject* obj) : Singleton(obj) {}

public:
	Player* addPlayer(const std::string& name, int id, bool isMain);
	Player* getPlayer(int id) const;
	Player* getMainPlayer() const;

	static void createPlayerTank(const Player* player);

	friend class GameObject;
	friend class Multiplayer;
};
