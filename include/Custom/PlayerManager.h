#pragma once

#include "Component.h"
#include "Singleton.h"

#include <boost/uuid/uuid.hpp>

class Tank;

class Player
{
	boost::uuids::uuid _id;
	std::string _name;
	Tank* _tank;

public:
	Player(boost::uuids::uuid id, const std::string& name, Tank* tank);

	boost::uuids::uuid id() const;
	std::string name() const;
	Tank* tank() const;

	friend class PlayerManager;
};

class PlayerManager : public Singleton<PlayerManager>
{
	std::vector<std::unique_ptr<Player>> players;

	PlayerManager(GameObject* obj) : Singleton(obj) {}

public:
	Player* addPlayer(const std::string& name, bool isMain, boost::uuids::uuid id = {});
	Player* getPlayer(boost::uuids::uuid id) const;
	Player* getMainPlayer() const;

	friend class GameObject;
};
