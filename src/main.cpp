#include "SDLHandler.h"

int main(int argv, char* args[])
{
	SDLHandler::initialize(500, 500);

	while (true)
	{
		if (!SDLHandler::update())
			break;
	}

	SDLHandler::quit();
	return 0;
}
