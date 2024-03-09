#include "SDLHandler.h"

int main(int argv, char* args[])
{
	SDLHandler::init(500, 500);

	SDLHandler::loop();

	SDLHandler::quit();
	return 0;
}
