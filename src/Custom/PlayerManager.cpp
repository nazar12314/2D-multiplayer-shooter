#include "PlayerManager.h"

#include <boost/uuid/uuid_generators.hpp>

#include "Camera.h"
#include "CameraFollow.h"
#include "MyMath.h"
#include "Tank.h"

Player::Player(int id, bool isMain, const std::string& name, Tank* tank) : _id(id), _isMain(isMain), _name(name), _tank(tank) {}

int Player::id() const { return _id; }
bool Player::isMain() const { return _isMain; }
std::string Player::name() const { return _name; }
Tank* Player::tank() const { return _tank; }

Player* PlayerManager::addPlayer(const std::string& name, int id, bool isMain)
{
	auto tank = GameObject::create("Player", Math::randomVec2(-5.0f, 5.0f))->addComponent<Tank>(isMain);
	auto player = std::make_unique<Player>(id, isMain, name, tank);

	if (isMain)
		Camera::getMain()->gameObject()->getComponent<CameraFollow>()->setTarget(tank->transform());

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
	return std::ranges::find_if(players, [](const auto& player) { return player->_isMain; })->get();
}
