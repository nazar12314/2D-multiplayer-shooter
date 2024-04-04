#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>

#include "Application.h"

void main_game()
{
	Application::start({800, 800});

	Application::loop();

	Application::quit();
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	main_game();
	return 0;
}
