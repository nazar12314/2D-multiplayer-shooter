#include <SDL_net.h>
#include <iostream>
#include <string>
#include <thread>

#include "Application.h"
#include "Multiplayer/Server.h"
#include "Multiplayer/Client.h"
#include "Assets.h"

void runServer(Uint16 port)
{
	Server server(port);
	if (!server.init())
	{
		std::cerr << "Server failed to initialize." << std::endl;
		return;
	}
	std::cout << "Server running..." << std::endl;
	while (true)
	{
		server.acceptConnections();
		server.receiveMessages();
	}
}

void runClient(const std::string& host, Uint16 port)
{
	Client client(host, port);
	if (!client.connect())
	{
		std::cerr << "Failed to connect to server." << std::endl;
		return;
	}
	std::cout << "Connected to server." << std::endl;

	std::string message;
	while (true)
	{
		std::cout << "Enter message: ";
		std::getline(std::cin, message);
		if (client.send(message))
		{
			std::cout << "Message sent." << std::endl;
		}

		std::string received = client.receive();
		if (!received.empty())
		{
			std::cout << "Received: " << received << std::endl;
		}
	}

	client.disconnect();
}

bool main_net()
{
	if (SDLNet_Init() != 0)
	{
		std::cerr << "SDLNet_Init Error: " << SDLNet_GetError() << std::endl;
		return true;
	}

	int choice;
	std::cout << "Run as server(1) or client(2): ";
	std::cin >> choice;
	std::cin.ignore();

	if (choice == 1)
	{
		std::thread serverThread(runServer, 1234);
		serverThread.join();
	}
	else if (choice == 2)
	{
		runClient("127.0.0.1", 1234);
	}
	else
	{
		std::cout << "Invalid choice." << std::endl;
	}

	SDLNet_Quit();
	return false;
}

void main_game()
{
	Application::start({800, 800});

	Application::loop();

	Application::quit();
}

int main(int argc, char* argv[])
{
	//main_net();

	main_game();

	return 0;
}
