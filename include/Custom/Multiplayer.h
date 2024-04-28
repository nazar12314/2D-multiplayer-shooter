#pragma once

#include "Singleton.h"
#include "Multiplayer/Server.h"

class Client;

class Multiplayer : public Singleton<Multiplayer>
{
	bool _isServer = false;

	std::unique_ptr<Client> _client = nullptr;
	std::unique_ptr<Server<net::MessageType>> _server = nullptr;

	Multiplayer(GameObject* obj, bool isServer);

	void start() override;

	void fixedUpdate() override;
	void updateServer() const;
	void updateClient() const;

	friend class GameObject;
};
