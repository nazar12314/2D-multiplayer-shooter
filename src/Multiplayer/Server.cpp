#include "Multiplayer/Server.h"

Server::Server(Uint16 port) {

    if (SDLNet_Init() < 0) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
    }

    if (SDLNet_ResolveHost(&ip, NULL, port) < 0) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
    }

    serverSocket = SDLNet_TCP_Open(&ip);
    if (!serverSocket) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
    }

    socketSet = SDLNet_AllocSocketSet(16); // For example, up to 16 sockets; adjust as needed
    if (!socketSet) {
        std::cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << std::endl;
    } else {
        if (SDLNet_TCP_AddSocket(socketSet, serverSocket) == -1) {
            std::cerr << "SDLNet_TCP_AddSocket: " << SDLNet_GetError() << std::endl;
        }
    }
}

Server::~Server() {
    for (auto& socket : clientSockets) {
        SDLNet_TCP_Close(socket);
    }
    SDLNet_TCP_Close(serverSocket);
    SDLNet_Quit();
}

bool Server::init() {

    return true;
}

void Server::acceptConnections() {

    TCPsocket clientSocket = SDLNet_TCP_Accept(serverSocket);
    if (clientSocket) {
        clientSockets.push_back(clientSocket);
        std::cout << "New connection accepted.\n";
    }
    if (clientSocket) {
        if (SDLNet_TCP_AddSocket(socketSet, clientSocket) == -1) {
            std::cerr << "Failed to add client socket to set: " << SDLNet_GetError() << std::endl;
        }
    }
}

void Server::receiveMessages() {
    if (SDLNet_CheckSockets(socketSet, 0) > 0) { // 0 timeout for non-blocking
        char message[1024];
        for (auto& socket : clientSockets) {
            if (SDLNet_SocketReady(socket)) {
                if (SDLNet_TCP_Recv(socket, message, 1024) > 0) {
                    std::string receivedMessage(message);
                    std::cout << "Received: " << receivedMessage << std::endl;
                    sendToAll(receivedMessage);
                }
            }
        }
    }
}


void Server::sendToAll(const std::string& message) {

    for (auto& socket : clientSockets) {
        SDLNet_TCP_Send(socket, message.c_str(), message.length() + 1); // +1 for NULL terminator
    }
}
