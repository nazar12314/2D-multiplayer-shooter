#include "Application.h"
#include "Camera.h"
#include "CircleCollider.h"

int main(int argv, char* args[])
{
	Application::init({500, 500});

	Application::loop();

	Application::quit();
	return 0;
}
