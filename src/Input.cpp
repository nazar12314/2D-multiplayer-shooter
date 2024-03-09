#include "Input.h"

#include "Application.h"

void Input::updateInput()
{
	if (!Application::windowFocused) return;
}

void Input::handleInputEvent(const SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_F11)
		{
			Application::isFullscreen = !Application::isFullscreen;
			SDL_SetWindowFullscreen(Application::window, Application::isFullscreen ? 1 : 0);
		}
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			Application::windowFocused = !Application::windowFocused;
			if (Application::windowFocused)
				SDL_SetRelativeMouseMode(SDL_TRUE);
			else
				SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}
}
