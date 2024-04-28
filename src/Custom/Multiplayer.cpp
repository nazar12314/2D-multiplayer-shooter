#include "Multiplayer.h"

#include "DOTween.h"
#include "PlayerManager.h"
#include "Tank.h"
#include "Transform.h"
#include "glm/gtx/string_cast.hpp"
#include "Multiplayer/Client.h"
#include "Multiplayer/Server.h"

Multiplayer::Multiplayer(GameObject* obj, bool isServer): Singleton(obj), _isServer(isServer) {}

void Multiplayer::start()
{
	if (_isServer)
		_server = std::make_unique<Server<net::MessageType>>(6000);

	_client = std::make_unique<Client<net::MessageType>>("127.0.0.1", "6000");
	addMainPlayer();
}
void Multiplayer::addMainPlayer() const
{
	auto self = PlayerManager::instance()->addPlayer("Player 1", true);

	net::PlayerConnectionData data;
	data.id = self->id();
	memcpy(data.name, self->name().c_str(), self->name().size());
	data.name[self->name().size()] = '\0';

	_client->send_message(net::MessageType::ADD_PLAYER, data);
}

void Multiplayer::fixedUpdate()
{
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

		auto body = msg_ptr->msg->get_body<net::PlayerGameData>();
		_server->message_clients(msg_ptr->msg->header.id, body, msg_ptr->owner);


		std::cout << "Server received message: " << msg_ptr->msg->header.id << std::endl;
	}
}
void Multiplayer::syncNewPlayer(const net::OwnedMessage<net::MessageType>* msg_ptr) const
{
	auto mainPlayer = PlayerManager::instance()->getMainPlayer();
	for (const auto& player : PlayerManager::instance()->players)
	{
		if (player->id() == mainPlayer->id()) continue;

		net::PlayerConnectionData data;
		data.id = player->id();
		memcpy(data.name, player->name().c_str(), player->name().size());

		_server->message_client(msg_ptr->owner, net::MessageType::ADD_PLAYER, data);
	}
}

void Multiplayer::updateClient() const
{
	updateClientSend();
	updateClientReceive();
}
void Multiplayer::updateClientSend() const
{
	auto self = PlayerManager::instance()->getMainPlayer();

	net::PlayerGameData data;
	data.id = self->id();
	data.position = self->tank()->transform()->getPos();
	data.rotation = self->tank()->transform()->getRot();

	_client->send_message(net::MessageType::UPDATE_PLAYER, data);

	std::cout << "Client sent message: UPDATE_PLAYER: " << self->id() << std::endl;
}
void Multiplayer::updateClientReceive() const
{
	net::OwnedMessage<net::MessageType>* omsg_ptr;
	while (_client->message_queue.pop(omsg_ptr))
	{
		net::Message<net::MessageType>* msg_ptr = omsg_ptr->msg;

		switch (msg_ptr->header.id)
		{
		case net::MessageType::UPDATE_PLAYER: {
			auto body = msg_ptr->get_body<net::PlayerGameData>();

			auto player = PlayerManager::instance()->getPlayer(body.id);
			player->tank()->transform()->setPos(body.position);
			player->tank()->transform()->setRot(body.rotation);

			std::cout << "Client received message: UPDATE_PLAYER: " << body.id << std::endl;

			break;
		}
		case net::MessageType::ADD_PLAYER: {
			auto body = msg_ptr->get_body<net::PlayerConnectionData>();

			PlayerManager::instance()->addPlayer(body.name, false);

			std::cout << "Client received message: ADD_PLAYER: " << body.name << std::endl;
			break;
		}
		}
	}
}
