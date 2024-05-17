#include "PlayerManager.h"

#include "Camera.h"
#include "CameraFollow.h"
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
	auto tank = GameObject::create("Player", Math::randomVec2(-5.0f, 5.0f))->addComponent<Tank>(name, data.color, isMain);

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

void PlayerManager::preparePlayerRespawn(Tank* tank)
{
	Player* player = std::ranges::find_if(players, [tank](const auto& playerr) { return playerr->tank == tank; })->get();

	auto counter = GameObject::create("RespawnCounter", tank->transform()->pos())->addComponent<TextRenderer>();
	Tweener::floatTo(5, 0, 5, [counter](float value) { counter->setText(std::to_string((int)std::ceil(value))); })->setEase(EaseType::Linear)
		->onComplete([counter, player]
		{
			if (Multiplayer::isServer)
			{
				auto respawnPos = Math::randomVec2(-5.0f, 5.0f);
				player->tank->respawn(respawnPos);

				auto playerRespawnData = net::PlayerRespawnData {player->id, respawnPos};
				Multiplayer::instance()->sendToClients(net::MessageType::PLAYER_RESPAWN, playerRespawnData);
			}
			destroy(counter->gameObject());
		});
}
