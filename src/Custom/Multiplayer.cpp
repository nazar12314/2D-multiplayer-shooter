#include "Multiplayer.h"

#include "DOTween.h"
#include "PlayerManager.h"
#include "Tank.h"
#include "Transform.h"
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
	net::Message<net::MessageType>* msg_ptr;
	while (_server->message_queue.pop(msg_ptr))
	{
		auto body = msg_ptr->get_body<net::PlayerGameData>();
		_server->message_clients(msg_ptr->header.id, body);

		//std::cout << "Server received message: " << (int)msg_ptr->header.id << std::endl;
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
	memcpy(data.name, self->name().c_str(), self->name().size());
	data.position = self->tank()->transform()->getPos();
	data.rotation = self->tank()->transform()->getRot();

	_client->send_message(net::MessageType::UPDATE_PLAYER, data);
}
void Multiplayer::updateClientReceive() const
{
	net::Message<net::MessageType>* msg_ptr;
	while (_client->message_queue.pop(msg_ptr))
	{
		//auto body = msg_ptr->get_body<net::ObjectDescription>();
		//std::cout << "Client received message: " << body.id << std::endl;
		//std::cout << "Client received message: " << body.name << std::endl;

		switch (msg_ptr->header.id)
		{
		case net::MessageType::UPDATE_PLAYER: {
			auto body = msg_ptr->get_body<net::PlayerGameData>();

			auto player = PlayerManager::instance()->getPlayer(body.id);
			player->tank()->transform()->setPos(body.position);
			player->tank()->transform()->setRot(body.rotation);
			break;
		}
		case net::MessageType::ADD_PLAYER: {
			auto [id, name] = msg_ptr->get_body<net::PlayerConnectionData>();
			PlayerManager::instance()->addPlayer(name, false);
			break;
		}
		}
	}
}
