#include "SDLHandler.h"

#include <string>

#include "Application.h"
#include "Input.h"
#include "Multiplayer.h"
#include "SDL.h"
#include "SDL_ttf.h"

void SDLHandler::init(const glm::ivec2& windowSize)
{
	SDLHandler::windowSize = windowSize;
	windowAspectRatio = (float)windowSize.x / windowSize.y;

	std::string title = "2D Multiplayer Shooter" + (Multiplayer::isServerCONFIG ? std::string(" Server") : std::string(" Client"));
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_RESIZABLE);
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
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			windowSize = {e.window.data1, e.window.data2};
			windowAspectRatio = (float)windowSize.x / windowSize.y;
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
