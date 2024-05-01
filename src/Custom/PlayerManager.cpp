#include "PlayerManager.h"

#include "Camera.h"
#include "CameraFollow.h"
#include "Multiplayer.h"
#include "MyMath.h"
#include "Tank.h"
#include "Multiplayer/net_modules.h"

Player* PlayerManager::addPlayer(const net::AddPlayerData& data, bool isMain)
{
	auto name = std::string(data.name) + " " + std::to_string(data.id);
	auto tank = GameObject::create("Player", Math::randomVec2(-5.0f, 5.0f))->addComponent<Tank>(name, data.color, isMain);
	auto player = std::make_unique<Player>(data.id, isMain, name, data.color, tank);

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
		if (player->id == id)
			return player.get();
	}
	return nullptr;
}
Player* PlayerManager::getMainPlayer() const
{
	return std::ranges::find_if(players, [](const auto& player) { return player->isMain; })->get();
}
