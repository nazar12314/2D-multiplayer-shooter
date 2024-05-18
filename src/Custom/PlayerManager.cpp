#include "PlayerManager.h"

#include "Application.h"
#include "Camera.h"
#include "CameraFollow.h"
#include "MapManager.h"
#include "Multiplayer.h"
#include "MyMath.h"
#include "ScoreDisplayer.h"
#include "Tank.h"
#include "Multiplayer/net_modules.h"
#include "TextRenderer.h"
#include "Transform.h"
#include "Tweener.h"

Player* PlayerManager::addPlayer(const net::AddPlayerData& data, bool isMain)
{
	auto name = std::string(data.name) + " " + std::to_string(data.id);
	auto tank = GameObject::create("Player", getSpawnPos())->addComponent<Tank>(name, data.color, isMain);

	auto player = std::make_unique<Player>(data.id, isMain, name, data.color, tank);
	auto playerPtr = player.get();
	players.push_back(std::move(player));

	tank->setPlayer(playerPtr);

	if (isMain)
		Camera::getMain()->gameObject()->getComponent<CameraFollow>()->setTarget(tank->transform());

	ScoreDisplayer::instance()->addPlayerToScoreboard(playerPtr);

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

void PlayerManager::preparePlayerRespawn(const Tank* tank) const
{
	Player* player = tank->player();

	auto counter = GameObject::create("RespawnCounter", tank->transform()->pos())->addComponent<TextRenderer>();
	Tweener::floatTo(5, 0, 5, [counter](float value) { counter->setText(std::to_string((int)std::ceil(value))); })->setEase(EaseType::Linear)
		->onComplete([counter, player, this]
		{
			if (Multiplayer::isServer)
			{
				auto respawnPos = getSpawnPos(player);
				player->tank->respawn(respawnPos);

				auto playerRespawnData = net::PlayerRespawnData {player->id, respawnPos};
				Multiplayer::instance()->sendToClients(net::MessageType::PLAYER_RESPAWN, playerRespawnData);
			}
			destroy(counter->gameObject());
		});
}
glm::vec2 PlayerManager::getSpawnPos(Player* player) const
{
	glm::vec2 spawnPos;
	auto counter = 0;
	auto range = MapManager::MAP_SIZE / 2 - 3;
	do
	{
		spawnPos = Math::randomVec2(-range, range);
		counter++;
	}
	while (counter < 100 && std::ranges::any_of(players, [spawnPos, this, player](const auto& p)
	{
		return player != p.get() && glm::distance(p->tank->transform()->pos(), spawnPos) < MIN_DIS_TO_OTHER;
	}));
	return spawnPos;
}
