//#include "Multiplayer/Client.h"
//#include <iostream>
//#include <cstring>
//
//Client::Client(const std::string& host, Uint16 port) {
//    if (SDLNet_Init() < 0) {
//        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
//    }
//
//    if (SDLNet_ResolveHost(&ip, host.c_str(), port) == -1) {
//        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
//    }
//    socketSet = SDLNet_AllocSocketSet(1);
//    if (!socketSet) {
//        std::cerr << "Failed to allocate the socket set: " << SDLNet_GetError() << std::endl;
//    }
//}
//
//Client::~Client() {
//    disconnect();
//    if (socketSet) {
//        SDLNet_FreeSocketSet(socketSet); // Free the socket set
//    }
//}
//
//bool Client::connect() {
//
//    socket = SDLNet_TCP_Open(&ip);
//    if (!socket) {
//        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
//        return false;
//    }
//    isConnected = true;
//    if (isConnected && socketSet) {
//        if (SDLNet_TCP_AddSocket(socketSet, socket) == -1) {
//            std::cerr << "Failed to add the client socket to the set: " << SDLNet_GetError() << std::endl;
//        }
//    }
//    return isConnected;
//
//}
//
//void Client::disconnect() {
//    if (socket) {
//        SDLNet_TCP_Close(socket);
//        socket = nullptr;
//    }
//    if (isConnected) {
//        SDLNet_Quit();
//        isConnected = false;
//    }
//}
//
//bool Client::send(const std::string& message) {
//    int result = SDLNet_TCP_Send(socket, message.c_str(), message.length() + 1);
//    return result >= static_cast<int>(message.length());
//}
//
//std::string Client::receive() {
//    if (SDLNet_CheckSockets(socketSet, 0) > 0) { // 0 timeout for non-blocking check
//        if (SDLNet_SocketReady(socket)) {
//            char message[1024];
//            std::memset(message, 0, sizeof(message));
//            int receivedLength = SDLNet_TCP_Recv(socket, message, 1024 - 1);
//
//            if (receivedLength <= 0) {
//                // Handle disconnect or error
//                return "";
//            }
//
//            return std::string(message);
//        }
//    }
//    return ""; // Return an empty string if no data is available
//}
//
