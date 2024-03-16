#include "Multiplayer.h"
#include "SDL_net.h"
#include <SDL.h>
#include <iostream>
#include <cstring>

int main(int argv, char* args[])
{
    int n;
    std::cout << "Server(0/1): ";
    std::cin >> n;

    if (n){
        const int PORT = 1234;
        const int BROADCAST_INTERVAL = 5000;
        SDL_Init(SDL_INIT_EVERYTHING);
        SDLNet_Init();

        IPaddress ip;
        // Use SDLNet_ResolveHost to fill the IPaddress structure (here we use NULL for broadcasting)
        SDLNet_ResolveHost(&ip, NULL, PORT);

        // Open the socket
        UDPsocket udpsock = SDLNet_UDP_Open(PORT); // Use the same port for listening to messages
        if (!udpsock) {
            std::cerr << "SDLNet_UDP_Open: " << SDLNet_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        // Allocate packet for broadcasting and receiving
        UDPpacket *packet = SDLNet_AllocPacket(512); // Adjust size as needed
        if (!packet) {
            std::cerr << "SDLNet_AllocPacket: " << SDLNet_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        // Setup for broadcasting
        strcpy((char *)packet->data, "Server Here!");
        packet->address.host = ip.host; // Broadcast address
        packet->address.port = ip.port;
        packet->len = strlen((char *)packet->data) + 1;

        while (true) {
            // Broadcast server presence
            if (SDLNet_UDP_Send(udpsock, -1, packet) == 0) {
                std::cerr << "SDLNet_UDP_Send: " << SDLNet_GetError() << std::endl;
            }

            // Check for incoming messages
            while (SDLNet_UDP_Recv(udpsock, packet)) {
                char clientIP[15];
                SDLNet_Write32(packet->address.host, clientIP);
                Uint16 clientPort = packet->address.port;

                std::cout << "Received message from " << SDLNet_ResolveIP(&packet->address) << ":" << clientPort << " - " << packet->data << std::endl;
                // Here you might want to handle the message or respond to the client
            }

            SDL_Delay(BROADCAST_INTERVAL);
        }

        SDLNet_FreePacket(packet);
        SDLNet_UDP_Close(udpsock);
        SDLNet_Quit();
        SDL_Quit();
    }
    else{
        SDL_Init(SDL_INIT_EVERYTHING);
        SDLNet_Init();
        const int PORT = 1234;
        IPaddress ip;
        UDPsocket udpsock = SDLNet_UDP_Open(PORT);
        if (!udpsock) {
            std::cerr << "SDLNet_UDP_Open: " << SDLNet_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        UDPpacket *packet = SDLNet_AllocPacket(512); // Adjust size as needed
        if (!packet) {
            std::cerr << "SDLNet_AllocPacket: " << SDLNet_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        std::unordered_map<int, IPaddress> servers;
        int serverCount = 0;
        std::cout << "Listening for servers\n";


        if (SDLNet_UDP_Recv(udpsock, packet)) {
            servers[++serverCount] = packet->address;
            std::cout << "Server " << serverCount << ": " << SDLNet_ResolveIP(&packet->address) << std::endl;

        }


        if (servers.empty()) {
            std::cout << "No servers found." << std::endl;
        } else {
            std::cout << "Enter the number of the server you want to connect to: ";
            int choice;
            std::cin >> choice;

            if (servers.find(choice) != servers.end()) {
                std::cout << "Enter message to send to server " << choice << ": ";
                std::string message;
                std::cin.ignore(); // Flush newline character from the stream
                std::getline(std::cin, message);

                // Prepare packet for sending message
                strncpy((char *)packet->data, message.c_str(), 512);
                packet->len = message.length() + 1;
                packet->address = servers[choice];

                if (!SDLNet_UDP_Send(udpsock, -1, packet)) {
                    std::cerr << "SDLNet_UDP_Send: " << SDLNet_GetError() << std::endl;
                } else {
                    std::cout << "Message sent to server " << choice << "." << std::endl;
                }
            } else {
                std::cout << "Invalid server selection." << std::endl;
            }
        }

        SDLNet_FreePacket(packet);
        SDLNet_UDP_Close(udpsock);
        SDLNet_Quit();
        SDL_Quit();
    }
    return 0;
}