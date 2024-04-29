#include "Multiplayer.h"

#include "DOTween.h"
#include "PlayerManager.h"
#include "SpriteRenderer.h"
#include "Tank.h"
#include "Transform.h"
#include "glm/gtx/string_cast.hpp"
#include "Multiplayer/Client.h"
#include "Multiplayer/Server.h"
#include "TankRemoteController.h"

Multiplayer::Multiplayer(GameObject* obj, bool isServer): Singleton(obj), _isServer(isServer) {}

void Multiplayer::start()
{
	if (_isServer)
		_server = std::make_unique<Server<net::MessageType>>(1234);

	_client = std::make_unique<Client<net::MessageType>>("127.0.0.1", "1234");
	addMainPlayer();
}
void Multiplayer::addMainPlayer() const
{
	auto mainPlayer = PlayerManager::instance()->addPlayer("Player 1", true);

	net::PlayerConnectionData data;
	data.id = mainPlayer->id();
	memcpy(data.name, mainPlayer->name().c_str(), mainPlayer->name().size());
	data.name[mainPlayer->name().size()] = '\0';

	_client->send_message(net::MessageType::ADD_PLAYER, data);
}

void Multiplayer::fixedUpdate()
{
	if (updatesCounter++ < updatesPerSync) return;
	updatesCounter = 0;

	if (_isServer)
		updateServer();

	updateClient();
}
void Multiplayer::updateServer() const
{
	updateServerSyncClients();
}
void Multiplayer::updateServerSyncClients() const
{
	net::OwnedMessage<net::MessageType>* msg_ptr;
	while (_server->message_queue.pop(msg_ptr))
	{
		if (msg_ptr->msg->header.id == net::MessageType::ADD_PLAYER)
			syncNewPlayer(msg_ptr);
		//else if (msg_ptr->msg->header.id == net::MessageType::UPDATE_PLAYER)
		//{
		//	auto body = msg_ptr->msg->get_body<net::PlayerGameData>();
		//	std::cout << body.id << '\n';
		//}

		_server->message_clients(*msg_ptr->msg, msg_ptr->owner);

		delete msg_ptr;
	}

}
void Multiplayer::syncNewPlayer(const net::OwnedMessage<net::MessageType>* msg_ptr) const
{
	auto body = msg_ptr->msg->get_body<net::PlayerConnectionData>();
	for (const auto& player : PlayerManager::instance()->players)
	{
		if (player->id() == body.id) continue;

		net::PlayerConnectionData data;
		data.id = player->id();
		memcpy(data.name, player->name().c_str(), player->name().size());
		data.name[player->name().size()] = '\0';

		_server->message_client(msg_ptr->owner, net::MessageType::ADD_PLAYER, data);
	}
}

void Multiplayer::updateClient() const
{
	updateClientReceive();
	updateClientSend();
}
void Multiplayer::updateClientSend() const
{
	auto self = PlayerManager::instance()->getMainPlayer();

	net::PlayerGameData data;
	data.id = self->id();
	data.position = self->tank()->transform()->getPos();
	data.rotation = self->tank()->transform()->getRot();
	data.gunRotation = self->tank()->gunPivot()->getRot();
	data.shoot = self->tank()->didShoot;
	self->tank()->didShoot = false;

	_client->send_message(net::MessageType::UPDATE_PLAYER, data);

	//std::cout << "Client sent message: UPDATE_PLAYER: " << data.id << std::endl;
}
void Multiplayer::updateClientReceive() const
{
	net::OwnedMessage<net::MessageType>* o_msg_ptr;
	while (_client->message_queue.pop(o_msg_ptr))
	{
		net::Message<net::MessageType>* msg_ptr = o_msg_ptr->msg;

		switch (msg_ptr->header.id)
		{
		case net::MessageType::UPDATE_PLAYER: {
			auto body = msg_ptr->get_body<net::PlayerGameData>();

			auto player = PlayerManager::instance()->getPlayer(body.id);
			if (!player)
			{
				std::cout << "Player not found: " << body.id << std::endl;
				continue;
			}

			auto remoteController = player->tank()->getComponent<TankRemoteController>();
			remoteController->moveTo(body.position);
			remoteController->rotateTo(body.rotation);
			remoteController->rotateGunTo(body.gunRotation);
			if (body.shoot) remoteController->shoot();

			//std::cout << "Client received message: UPDATE_PLAYER: " << body.id << std::endl;

			break;
		}
		case net::MessageType::ADD_PLAYER: {
			auto body = msg_ptr->get_body<net::PlayerConnectionData>();

			PlayerManager::instance()->addPlayer(body.name, false, body.id);

			std::cout << "Client received message: ADD_PLAYER: " << body.name << std::endl;
			break;
		}
		}
	}
}
