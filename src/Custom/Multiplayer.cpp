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

	_client = std::make_unique<Client>("127.0.0.1", "6000");
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
		if (!msg_ptr) continue;
		auto objDesc = msg_ptr->get_body<net::ObjectDescription>();

		std::cout << "Object ID: " << objDesc.id << std::endl;
		std::cout << "Object Name: " << objDesc.name << std::endl;
		std::cout << "Object Position: (" << objDesc.position.x << ", " << objDesc.position.y << ")" << std::endl;
		std::cout << "Object Rotation: " << objDesc.rotation << std::endl;
		std::cout << std::endl;
	}
}

void Multiplayer::updateClient() const
{
	net::Message<net::MessageType> msg;
	msg.header.id = net::MessageType::UPDATE_PLAYER;

	auto self = PlayerManager::instance()->getMainPlayer();

	net::ObjectDescription objDesc;
	objDesc.id = self->id();
	objDesc.name = self->name();
	objDesc.position = self->tank()->transform()->getPos();
	objDesc.rotation = self->tank()->transform()->getRot();

	msg.set_body(objDesc);

	_client->send_message(net::MessageType::UPDATE_PLAYER, objDesc);
}
