#pragma once

#include "Singleton.h"
#include "Multiplayer/Server.h"

class Player;
template <typename T>
class Client;

class Multiplayer : public Singleton<Multiplayer>
{
	bool _isServer = false;

	std::unique_ptr<Client<net::MessageType>> _client = nullptr;
	std::unique_ptr<Server<net::MessageType>> _server = nullptr;

	Multiplayer(GameObject* obj, bool isServer);

	void start() override;
	void addMainPlayer() const;

	void fixedUpdate() override;

	void updateServer() const;
	void updateServerSyncClients() const;

	void updateClient() const;
	void updateClientSend() const;
	void updateClientReceive() const;

	friend class GameObject;
};
