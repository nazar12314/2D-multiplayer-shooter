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

	void serverReceive();
	void serverRegisterClient(const net::OwnedMessage<net::MessageType>* msg_ptr);
	static void serverHandlePlayerActions(const net::OwnedMessage<net::MessageType>* msg_ptr);

	void serverShareGameState() const;

	void updateClient();
	void clientSendActions() const;
	void clientReceive();

public:
	inline static bool isServerCONFIG = true;

	friend class GameObject;
};
