#include "PlayerManager.h"

#include <boost/uuid/uuid_generators.hpp>

#include "MyMath.h"
#include "Tank.h"

Player::Player(boost::uuids::uuid id, const std::string& name, Tank* tank) : _id(id), _name(name), _tank(tank) {}

boost::uuids::uuid Player::id() const { return _id; }
std::string Player::name() const { return _name; }
Tank* Player::tank() const { return _tank; }


Player* PlayerManager::addPlayer(const std::string& name, bool isMain, boost::uuids::uuid id)
{
	if (isMain)
		id = boost::uuids::random_generator()();

	auto tank = GameObject::create("Player", Math::randomVec2(-18.0f, 18.0f))->addComponent<Tank>(isMain);
	auto player = std::make_unique<Player>(id, name, tank);

	auto playerPtr = player.get();
	players.push_back(std::move(player));
	return playerPtr;
}
Player* PlayerManager::getPlayer(boost::uuids::uuid id) const
{
	for (const auto& player : players)
	{
		if (player->_id == id)
			return player.get();
	}
	return nullptr;
}
Player* PlayerManager::getMainPlayer() const
{
	return players[0].get();
}
