#pragma once

#include "Component.h"
#include "Singleton.h"

class Tank;

class Player
{
	int _id;
	std::string _name;
	Tank* _tank;

public:
	Player(int id, const std::string& name, Tank* tank);

	int id() const;
	std::string name() const;
	Tank* tank() const;

	friend class PlayerManager;
};

class PlayerManager : public Singleton<PlayerManager>
{
	inline static int nextId = 0;

	std::vector<std::unique_ptr<Player>> players;

	PlayerManager(GameObject* obj) : Singleton(obj) {}

public:
	Player* addPlayer(const std::string& name, bool isMain);
	Player* getPlayer(int id) const;
	Player* getMainPlayer() const;

	friend class GameObject;
};
