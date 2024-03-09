#include "SDLHandler.h"

#include "Application.h"
#include "Input.h"
#include "SDL.h"

void SDLHandler::handleEvents()
{
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
		{
			Application::windowSize = {e.window.data1, e.window.data2};
		}
	}
}
