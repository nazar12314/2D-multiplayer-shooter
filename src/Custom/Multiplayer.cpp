// ReSharper disable CppUseStructuredBinding
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
	registerClient();
}
void Multiplayer::registerClient() const
{
	std::string name = "Player";

	net::RegisterClientData data;
	memcpy(data.name, name.c_str(), name.size());
	data.name[name.size()] = '\0';

	_client->send_message(net::MessageType::REGISTER_CLIENT, data);
}

void Multiplayer::fixedUpdate()
{
	if (_updatesCounter++ < _updatesPerSync) return;
	_updatesCounter = 0;

	if (_isServer)
		updateServer();

	updateClient();
}
void Multiplayer::updateServer()
{
	updateServerSyncClients();
}
void Multiplayer::updateServerSyncClients()
{
	net::OwnedMessage<net::MessageType>* msg_ptr;
	while (_server->message_queue.pop(msg_ptr))
	{
		if (msg_ptr->msg->header.id == net::MessageType::REGISTER_CLIENT)
		{
			registerClient(msg_ptr);
		}
		else
		{
			_server->message_clients(*msg_ptr->msg, msg_ptr->owner);
		}

		delete msg_ptr;
	}
}
void Multiplayer::registerClient(const net::OwnedMessage<net::MessageType>* msg_ptr)
{
	auto newPlayerId = msg_ptr->owner->id;

	// Assign an id to the new client
	net::ClientAssignIdData assignIdData;
	assignIdData.id = newPlayerId;
	_server->message_client(msg_ptr->owner, net::MessageType::CLIENT_ASSIGN_ID, assignIdData);

	// Add newly connected player
	auto data = msg_ptr->msg->get_body<net::RegisterClientData>();
	ConnectedPlayer newPlayer = {newPlayerId, data.name};
	_connectedPlayers.push_back(newPlayer);

	// Add new player to all clients
	net::AddPlayerData addNewPlayerData;
	addNewPlayerData.id = newPlayerId;
	memcpy(addNewPlayerData.name, data.name, strlen(data.name));
	addNewPlayerData.name[strlen(data.name)] = '\0';

	_server->message_clients(net::MessageType::ADD_PLAYER, addNewPlayerData, msg_ptr->owner);

	// Add all players to the new client, including itself
	for (const auto& player : _connectedPlayers)
	{
		net::AddPlayerData addPlayerData;
		addPlayerData.id = player.id;
		memcpy(addPlayerData.name, player.name.c_str(), player.name.size());
		addPlayerData.name[player.name.size()] = '\0';

		_server->message_client(msg_ptr->owner, net::MessageType::ADD_PLAYER, addPlayerData);
	}
}

void Multiplayer::updateClient()
{
	updateClientReceive();
	if (_isConnected)
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
void Multiplayer::updateClientReceive()
{
	net::OwnedMessage<net::MessageType>* o_msg_ptr;
	while (_client->message_queue.pop(o_msg_ptr))
	{
		net::Message<net::MessageType>* msg_ptr = o_msg_ptr->msg;

		switch (msg_ptr->header.id)
		{
		case net::MessageType::CLIENT_ASSIGN_ID: {
			auto body = msg_ptr->get_body<net::ClientAssignIdData>();

			_client->set_id(body.id);

			std::cout << "Client received message: CLIENT_ASSIGN_ID: " << body.id << std::endl;
			break;
		}
		case net::MessageType::ADD_PLAYER: {
			auto body = msg_ptr->get_body<net::AddPlayerData>();

			bool isMain = body.id == _client->id();
			PlayerManager::instance()->addPlayer(body.name, body.id, isMain);

			if (isMain) _isConnected = true;

			std::cout << "Client received message: ADD_PLAYER: " << body.name << std::endl;
			break;
		}
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
		}
	}
}
