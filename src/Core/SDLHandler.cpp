#include "SDLHandler.h"

#include "Application.h"
#include "Input.h"
#include "SDL.h"
#include "SDL2_ttf.h"

void SDLHandler::init(const glm::ivec2& windowSize)
{
	SDLHandler::windowSize = windowSize;
	windowAspectRatio = (float)windowSize.x / windowSize.y;

	window = SDL_CreateWindow("2D Multiplayer Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	surface = SDL_GetWindowSurface(window);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	TTF_Init();
}

void SDLHandler::handleEvents()
{
	Input::clear();

	SDL_Event e {};
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			Application::doQuit = true;
			return;
		}

		Input::handleInputEvent(e);

		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
			windowSize = {e.window.data1, e.window.data2};
	}
}

void SDLHandler::uninit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
