#ifndef CLIENT_H
#define CLIENT_H

#include <SDL_net.h>
#include <string>

class Client {
public:
    Client(const std::string& host, Uint16 port);
    ~Client();
    bool connect();
    void disconnect();
    bool send(const std::string& message);
    std::string receive();

private:
    TCPsocket socket = nullptr;
    SDLNet_SocketSet socketSet = nullptr;
    IPaddress ip;
    bool isConnected = false;
};

#endif // CLIENT_H
