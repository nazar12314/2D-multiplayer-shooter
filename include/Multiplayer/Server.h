#ifndef SERVER_H
#define SERVER_H

#include <SDL_net.h>
#include <iostream>
#include <string>
#include <vector>

class Server {
public:
    Server(Uint16 port);
    ~Server();
    bool init();
    void acceptConnections();
    void receiveMessages();
    void sendToAll(const std::string& message);

private:
    IPaddress ip;
    TCPsocket serverSocket;
    std::vector<TCPsocket> clientSockets;
    SDLNet_SocketSet socketSet;
};

#endif // SERVER_H
