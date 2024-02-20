#include "Input.h"

#include "SDLHandler.h"

void Input::updateInput()
{
	if (!SDLHandler::windowFocused) return;
}

void Input::handleSDLEvent(const SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_F11)
		{
			SDLHandler::isFullscreen = !SDLHandler::isFullscreen;
			SDL_SetWindowFullscreen(SDLHandler::window, SDLHandler::isFullscreen ? 1 : 0);
		}
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			SDLHandler::windowFocused = !SDLHandler::windowFocused;
			if (SDLHandler::windowFocused)
				SDL_SetRelativeMouseMode(SDL_TRUE);
			else
				SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}
}
