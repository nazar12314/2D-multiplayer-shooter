#include "SDLHandler.h"

#include "Input.h"

void SDLHandler::initialize(int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		printf("error initializing SDL: %s\n", SDL_GetError());

	window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}
bool SDLHandler::update()
{
	while (SDL_PollEvent(&event))
	{
		Input::handleSDLEvent(event);

		if (event.type == SDL_QUIT)
			return false;
	}
	return true;
}

void SDLHandler::quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
