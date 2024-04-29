#include "PlayerManager.h"

#include <boost/uuid/uuid_generators.hpp>

#include "MyMath.h"
#include "Tank.h"

Player::Player(int id, const std::string& name, Tank* tank) : _id(id), _name(name), _tank(tank) {}

int Player::id() const { return _id; }
std::string Player::name() const { return _name; }
Tank* Player::tank() const { return _tank; }


Player* PlayerManager::addPlayer(const std::string& name, bool isMain, int id)
{
	if (isMain)
		id = Math::randomInt(0, 999999999);

	auto tank = GameObject::create("Player", Math::randomVec2(-5.0f, 5.0f))->addComponent<Tank>(isMain);
	auto player = std::make_unique<Player>(id, name, tank);

	auto playerPtr = player.get();
	players.push_back(std::move(player));
	return playerPtr;
}
Player* PlayerManager::getPlayer(int id) const
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
