#include "Multiplayer.h"

bool initSDLNet() {
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        return false;
    }
    return true;
}

int server() {
    if (!initSDLNet()) {
        return 1;
    }

    IPaddress ip;
    TCPsocket server;

    if (SDLNet_ResolveHost(&ip, NULL, 1234) == -1) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return 1;
    }

    server = SDLNet_TCP_Open(&ip);
    if (!server) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return 1;
    }

    std::cout << "Server is running and waiting for connections...\n";

    while (true) {
        TCPsocket client = SDLNet_TCP_Accept(server);
        if (client) {
            IPaddress* remoteIp = SDLNet_TCP_GetPeerAddress(client);
            if (!remoteIp) {
                std::cerr << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << std::endl;
            } else {
                std::cout << "Client connected: " << SDLNet_Read32(&remoteIp->host) << std::endl;

                const char* message = "Welcome to the 2D Tanks Game!";
                SDLNet_TCP_Send(client, message, strlen(message) + 1);
                SDLNet_TCP_Close(client);
            }
        }
    }

    SDLNet_TCP_Close(server);
    SDLNet_Quit();
    return 0;
}

int client() {
    if (!initSDLNet()) {
        return 1;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, "localhost", 1234) == -1) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return 1;
    }

    TCPsocket client = SDLNet_TCP_Open(&ip);
    if (!client) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return 1;
    }

    char message[100];
    if (SDLNet_TCP_Recv(client, message, 100) > 0) {
        std::cout << "Message from server: " << message << std::endl;
    }

    SDLNet_TCP_Close(client);
    SDLNet_Quit();
    return 0;
}

