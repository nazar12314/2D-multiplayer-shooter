#include "Application.h"
#include "Multiplayer/Server.h"
#include "Multiplayer/Client.h"

//#define NETWORK
//#define SERVER

void main_game()
{
	Application::start({800, 800});

	Application::loop();

	Application::quit();
}

int main(int argc, char* argv[])
{
#ifdef NETWORK

#ifdef SERVER
    try {
        Server<int> server(12345);
        std::cout << "Server running on port 12345\n";

        while (true)
        {
            std::string input;
            std::cout << "Type 'print' to print all messages: ";
            std::cin >> input;

            if (input == "print")
            {
                server.print_messages();
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
#else
    Client client("127.0.0.1", "12345");

    try {
        net::ObjectDescription objDesc;
        objDesc.id = 11;
        objDesc.name = "Tank";
        objDesc.position = {10.0, 20.0};
        objDesc.rotation = 90.0;

        client.send_message<net::ObjectDescription>(net::MessageType::UPDATE_PLAYER, objDesc);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    client.close();
#endif

#else
	main_game();
#endif
	return 0;
}
