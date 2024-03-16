#include "Multiplayer.h"
#include "SDL_net.h"

int main(int argv, char* args[])
{
    int n;
    std::cout << "Server(0/1): ";
    std::cin >> n;

    if (n){
        if (SDLNet_Init() < 0) {
            // Handle error
        }

        IPaddress broadcastAddr;
        UDPsocket udpSocket = SDLNet_UDP_Open(0); // 0 lets SDL_net choose the port
        SDLNet_ResolveHost(&broadcastAddr, "255.255.255.255", 1234); // 1234 is the client's listening port

        UDPpacket *packet = SDLNet_AllocPacket(512); // Adjust size as needed
        strcpy((char *)packet->data, "Server Here!");
        packet->len = strlen((char *)packet->data) + 1;
        packet->address.host = broadcastAddr.host; // Broadcast address
        packet->address.port = broadcastAddr.port; // Client's listening port

        SDLNet_UDP_Send(udpSocket, -1, packet); // -1 means send to all on the channel

        SDLNet_FreePacket(packet);
        SDLNet_UDP_Close(udpSocket);
        SDLNet_Quit();

        return 0;
    }
    else{
        if (SDLNet_Init() < 0) {
            // Handle error
        }

        UDPsocket udpSocket = SDLNet_UDP_Open(1234); // Listening port
        UDPpacket *packet = SDLNet_AllocPacket(512); // Adjust size as needed

        while (true) {
            if (SDLNet_UDP_Recv(udpSocket, packet)) {
                printf("Received message: %s\n", (char *)packet->data);
                // Process packet data
            }
        }

        SDLNet_FreePacket(packet);
        SDLNet_UDP_Close(udpSocket);
        SDLNet_Quit();

        return 0;
    }
    return 0;
}