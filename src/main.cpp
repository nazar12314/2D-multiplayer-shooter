#include "Application.h"
#include "Multiplayer.h"
#include "Multiplayer/Client.h"

void main_game()
{
	std::cout << "Server (1) or Client (0)? ";
	std::cin >> Multiplayer::isServerCONFIG;

	Application::start({800, 800});

	Application::loop();

	Application::quit();
}

int main(int argc, char* argv[])
{
	main_game();

	return 0;
}
