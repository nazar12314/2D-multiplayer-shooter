#include "SDLHandler.h"

#include "Input.h"
#include "MyTime.h"
#include "Object.h"

void SDLHandler::init(int width, int height)
{
	initSDL(width, height);
	initScene();

	Object::startAll();
}
void SDLHandler::initSDL(int width, int height)
{
	window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	surface = SDL_GetWindowSurface(window);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}
void SDLHandler::initScene() {}

void SDLHandler::loop()
{
	int frame = 0;
	while (true)
	{
		bool ok = update();
		if (!ok) break;
		if (doQuit) break;

		auto rect = new SDL_Rect {50, 50, 100, 100};
		SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, 122, 122, 122));
		SDL_UpdateWindowSurface(window);
		frame++;
	}
}

bool SDLHandler::update()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			return false;

		Input::handleSDLEvent(event);
		Time::updateTime();
		Object::updateAll();
	}
	return true;
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
