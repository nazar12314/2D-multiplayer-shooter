#include "SDLHandler.h"

#include "Input.h"
#include "MyTime.h"
#include "Object.h"
#include "Renderer.h"
#include "Texture.h"

void SDLHandler::init(int width, int height)
{
	initSDL(width, height);
	initScene();

	Object::startAll();
}
void SDLHandler::initSDL(int width, int height)
{
	window = SDL_CreateWindow("2D Multiplayer Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	surface = SDL_GetWindowSurface(window);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}
void SDLHandler::initScene()
{
	auto tex = new Texture("sprites/circle.png");
	auto obj1 = new Graphical({50, 50}, 0, tex);
}

void SDLHandler::loop()
{
	int frame = 0;
	while (true)
	{
		Input::handleSDLEvents();
		Time::tick();
		Object::updateAll();

		if (doQuit) break;

		SDL_RenderClear(renderer);
		Renderer::draw();
		SDL_RenderPresent(renderer);

		frame++;
	}
}

void SDLHandler::quit()
{
	Object::destroyAll();
	uninitSDL();
}
void SDLHandler::uninitSDL()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
