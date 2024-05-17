#pragma once

#include "Singleton.h"
#include "Multiplayer/Server.h"
#include "Multiplayer/Client.h"

class Player;

class Multiplayer : public Singleton<Multiplayer>
{
	struct ConnectedPlayer
	{
		int id;
		std::string name;
		Color color;
	};

	std::vector<ConnectedPlayer> _connectedPlayers;

	std::unique_ptr<Client<net::MessageType>> _client = nullptr;
	std::unique_ptr<Server<net::MessageType>> _server = nullptr;

	int _updatesPerSync = 1;
	int _updatesCounter = 0;

	bool _isConnected = false;

	Multiplayer(GameObject* obj);

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
	inline static bool isServer = true;
	inline static std::string serverIP = "127.0.0.1";
	inline static std::string serverPort = "1234";

	template<typename T>
	void sendToClients(net::MessageType type, T data) const;

	friend class GameObject;
};

template <typename T> void Multiplayer::sendToClients(net::MessageType type, T data) const
{
	_server->message_clients(type, data);
}
