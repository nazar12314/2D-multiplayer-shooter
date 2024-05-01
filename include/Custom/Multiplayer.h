#pragma once

#include "Singleton.h"
#include "Multiplayer/Server.h"

class Player;
template <typename T> class Client;

class Multiplayer : public Singleton<Multiplayer>
{
	struct ConnectedPlayer
	{
		int id;
		std::string name;
		Color color;
	};

	bool _isServer = false;
	std::vector<ConnectedPlayer> _connectedPlayers;

	std::unique_ptr<Client<net::MessageType>> _client = nullptr;
	std::unique_ptr<Server<net::MessageType>> _server = nullptr;

	int _updatesPerSync = 1;
	int _updatesCounter = 0;

	bool _isConnected = false;

	Multiplayer(GameObject* obj, bool isServer);

	void start() override;
	void registerClient() const;

	void fixedUpdate() override;

	void updateServer();
	void updateServerSyncClients();
	void registerClient(const net::OwnedMessage<net::MessageType>* msg_ptr);

	void updateClient();
	void updateClientSend() const;
	void updateClientReceive();

public:
	inline static bool isServerCONFIG = true;

	const std::unique_ptr<Client<net::MessageType>>& client() const;

	friend class GameObject;
};
