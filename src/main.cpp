#include "Application.h"

void main_game()
{
	Application::start({800, 800});

	Application::loop();

	Application::quit();
}

int main(int argc, char* argv[])
{
	main_game();

	return 0;
}
