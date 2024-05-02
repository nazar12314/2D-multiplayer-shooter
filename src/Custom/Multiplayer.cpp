// ReSharper disable CppUseStructuredBinding
#include "Multiplayer.h"

#include "Tweener.h"
#include "PlayerManager.h"
#include "Rigidbody.h"
#include "Tank.h"
#include "Transform.h"
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
	memcpy(data.name, name.c_str(), name.size() + 1);
	data.color = Color::randomLight();

	_client->send_message(net::MessageType::REGISTER_CLIENT, data);
}

void Multiplayer::fixedUpdate()
{
	if (++_updatesCounter < _updatesPerSync) return;
	_updatesCounter = 0;

	if (_isServer)
		updateServer();

	updateClient();
}
void Multiplayer::updateServer()
{
	serverReceive();
	serverShareGameState();
}
void Multiplayer::serverReceive()
{
	net::OwnedMessage<net::MessageType>* msg_ptr;
	while (_server->message_queue.pop(msg_ptr))
	{
		switch (msg_ptr->msg->header.id)
		{
		case net::MessageType::REGISTER_CLIENT:
			serverRegisterClient(msg_ptr);
			break;
		case net::MessageType::PLAYER_ACTIONS: {
			serverHandlePlayerActions(msg_ptr);
			break;
		}
		default:
			_server->message_clients(*msg_ptr->msg, msg_ptr->owner);
			break;
		}

		delete msg_ptr;
	}
}
void Multiplayer::serverRegisterClient(const net::OwnedMessage<net::MessageType>* msg_ptr)
{
	auto newPlayerId = msg_ptr->owner->id;

	// Assign an id to the new client
	net::ClientAssignIdData assignIdData;
	assignIdData.id = newPlayerId;
	_server->message_client(msg_ptr->owner, net::MessageType::CLIENT_ASSIGN_ID, assignIdData);

	// Add newly connected player
	auto data = msg_ptr->msg->get_body<net::RegisterClientData>();
	ConnectedPlayer newPlayer = {newPlayerId, data.name, data.color};
	_connectedPlayers.push_back(newPlayer);

	// Add new player to all clients
	net::AddPlayerData addNewPlayerData;
	addNewPlayerData.id = newPlayerId;
	memcpy(addNewPlayerData.name, newPlayer.name.c_str(), newPlayer.name.size() + 1);
	addNewPlayerData.color = newPlayer.color;

	_server->message_clients(net::MessageType::ADD_PLAYER, addNewPlayerData, msg_ptr->owner);

	// Add all players to the new client, including itself
	for (const auto& player : _connectedPlayers)
	{
		net::AddPlayerData addPlayerData;
		addPlayerData.id = player.id;
		memcpy(addPlayerData.name, player.name.c_str(), player.name.size() + 1);
		addPlayerData.color = player.color;

		_server->message_client(msg_ptr->owner, net::MessageType::ADD_PLAYER, addPlayerData);
	}
}

void Multiplayer::serverHandlePlayerActions(const net::OwnedMessage<net::MessageType>* msg_ptr)
{
	auto body = msg_ptr->msg->get_body<net::PlayerRequestData>();

	auto player = PlayerManager::instance()->getPlayer(body.id);
	if (!player)
	{
		std::cout << "Player not found: " << body.id << std::endl;
		return;
	}

	auto remoteController = player->tank->remoteController();
	remoteController->applyMovement(body.moveBy);
	remoteController->applyRotation(body.rotateBy);
	remoteController->applyGunRotation(body.rotateGunBy);
	remoteController->applyShoot(body.shoot);
}

void Multiplayer::serverShareGameState() const
{
	for (const auto& player : _connectedPlayers)
	{
		auto playerObj = PlayerManager::instance()->getPlayer(player.id);
		if (!playerObj) continue;

		net::PlayerUpdateData data;
		data.id = player.id;
		data.position = playerObj->tank->transform()->pos();
		data.rotation = playerObj->tank->transform()->rot();
		data.gunRotation = playerObj->tank->gunPivot()->rot();
		data.shoot = playerObj->tank->didShoot;
		playerObj->tank->didShoot = false;
		data.velocity = playerObj->tank->rb()->velocity();
		data.angularVelocity = playerObj->tank->rb()->angularVelocity();

		_server->message_clients(net::MessageType::PLAYER_UPDATE, data);
	}
}

void Multiplayer::updateClient()
{
	clientReceive();
	if (_isConnected)
		clientSendActions();
}
void Multiplayer::clientSendActions() const
{
	auto self = PlayerManager::instance()->getMainPlayer();

	net::PlayerRequestData data;
	data.id = self->id;
	data.moveBy = self->tank->remoteController()->getAndResetRequestedMovement();
	data.rotateBy = self->tank->remoteController()->getAndResetRequestedRotation();
	data.rotateGunBy = self->tank->remoteController()->getAndResetRequestedGunRotation();
	data.shoot = self->tank->remoteController()->getAndResetRequestedShoot();

	_client->send_message(net::MessageType::PLAYER_ACTIONS, data);

	//std::cout << "Client sent message: UPDATE_PLAYER: " << data.id << std::endl;
}
void Multiplayer::clientReceive()
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
			PlayerManager::instance()->addPlayer(body, isMain);

			if (isMain) _isConnected = true;

			std::cout << "Client received message: ADD_PLAYER: " << body.name << std::endl;
			break;
		}
		case net::MessageType::PLAYER_UPDATE: {
			if (_isServer) break; // Ignore updates from server to itself
			auto body = msg_ptr->get_body<net::PlayerUpdateData>();

			auto player = PlayerManager::instance()->getPlayer(body.id);
			if (!player)
			{
				std::cout << "Player not found: " << body.id << std::endl;
				continue;
			}

			auto remoteController = player->tank->remoteController();
			remoteController->updatePos(body.position);
			remoteController->updateRot(body.rotation);
			remoteController->updateGunRot(body.gunRotation);
			remoteController->updateShoot(body.shoot);

			remoteController->updateVelocity(body.velocity);
			remoteController->updateAngularVelocity(body.angularVelocity);

			//std::cout << "Client received message: UPDATE_PLAYER: " << body.id << std::endl;
			break;
		}
		}
	}
}
