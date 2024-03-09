#include "Multiplayer.h"


int main(int argv, char* args[])
{
    int n;
    std::cout << "Server(0/1): ";
    std::cin >> n;

    if (n){
        server();
    }
    else{
        client();
    }
    return 0;
}