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
}

void Multiplayer::fixedUpdate()
{
	if (_isServer)
		updateServer();

	updateClient();
}
void Multiplayer::updateServer() const
{
	net::Message<net::MessageType>* msg_ptr;
	while (_server->message_queue.pop(msg_ptr))
	{
		_server->message_clients(msg_ptr->header.id, msg_ptr->body);
	}
}


void Multiplayer::updateClient() const
{
	updateClientSend();
}
void Multiplayer::updateClientSend() const
{
	net::Message<net::MessageType> msg;
	msg.header.id = net::MessageType::UPDATE_PLAYER;

	auto self = PlayerManager::instance()->getMainPlayer();

	net::ObjectDescription objDesc;
	objDesc.id = self->id();
	memcpy(objDesc.name, self->name().c_str(), self->name().size());
	objDesc.name[self->name().size()] = '\0';
	objDesc.position = self->tank()->transform()->getPos();
	objDesc.rotation = self->tank()->transform()->getRot();

	msg.set_body(objDesc);

	_client->send_message(net::MessageType::UPDATE_PLAYER, objDesc);
}
void Multiplayer::updateClientReceive() const
{
	net::Message<net::MessageType>* msg_ptr;
	while (_client->message_queue.pop(msg_ptr))
	{
		switch (msg_ptr->header.id)
		{
		case net::MessageType::UPDATE_PLAYER: {
			net::ObjectDescription objDesc;
			memcpy(&objDesc, msg_ptr->body.data(), msg_ptr->body.size());

			auto player = PlayerManager::instance()->getPlayer(objDesc.id);
			player->tank()->transform()->setPos(objDesc.position);
			player->tank()->transform()->setRot(objDesc.rotation);
		}
		break;
		}
	}
}
