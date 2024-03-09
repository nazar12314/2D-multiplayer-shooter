#include "Application.h"

#include "Camera.h"
#include "Input.h"
#include "MyTime.h"
#include "Object.h"
#include "Renderer.h"
#include "SDLHandler.h"
#include "Texture.h"

void Application::init(const glm::ivec2& windowSize)
{
	Application::windowSize = windowSize;

	initSDL(windowSize);
	initScene();

	Object::startAll();
}
void Application::initSDL(const glm::ivec2& windowSize)
{
	window = SDL_CreateWindow("2D Multiplayer Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	surface = SDL_GetWindowSurface(window);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}
void Application::initScene()
{
	auto tex = new Texture("sprites/circle.png");
	auto obj1 = new Graphical(tex, {50, 50});
}

void Application::loop()
{
	int frame = 0;
	while (true)
	{
		SDLHandler::handleEvents();
		Time::tick();
		Object::updateAll();

		if (doQuit) break;

		SDL_RenderClear(renderer);
		Camera::renderMain();
		SDL_RenderPresent(renderer);

		frame++;
	}
}

void Application::quit()
{
	Object::destroyAll();
	uninitSDL();
}
void Application::uninitSDL()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
