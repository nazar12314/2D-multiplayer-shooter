#include "Application.h"
#include "SDL.h"
#include "VectorDelayed.h"

int main(int argv, char* args[])
{
	Application::start({800, 800});

	Application::loop();

	Application::quit();
	return 0;
}
