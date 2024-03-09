#include "Input.h"

#include "Application.h"

void Input::clear()
{
	keysPressed.clear();
	keysReleased.clear();
	mouseButtonsPressed.clear();
	mouseButtonsReleased.clear();
	mouseMoved = false;
}

void Input::handleInputEvent(const SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		keysPressed.push_back(event.key.keysym.sym);
		keysCurrentlyDown.push_back(event.key.keysym.sym);

		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			Application::windowFocused = !Application::windowFocused;
			if (Application::windowFocused)
				SDL_SetRelativeMouseMode(SDL_TRUE);
			else
				SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		keysReleased.push_back(event.key.keysym.sym);
		keysCurrentlyDown.erase(std::remove(keysCurrentlyDown.begin(), keysCurrentlyDown.end(), event.key.keysym.sym), keysCurrentlyDown.end());
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		mouseButtonsPressed.push_back(event.button.button);
		mouseButtonsCurrentlyDown.push_back(event.button.button);
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		mouseButtonsReleased.push_back(event.button.button);
		mouseButtonsCurrentlyDown.erase(std::remove(mouseButtonsCurrentlyDown.begin(), mouseButtonsCurrentlyDown.end(), event.button.button), mouseButtonsCurrentlyDown.end());
	}
	else if (event.type == SDL_MOUSEMOTION)
	{
		mousePos.x = event.motion.x;
		mousePos.y = event.motion.y;

		mouseMoved = true;
	}
}

bool Input::wasKeyPressed(SDL_Keycode key)
{
	return std::ranges::find(keysPressed, key) != keysPressed.end();
}
bool Input::wasKeyReleased(SDL_Keycode key)
{
	return std::ranges::find(keysReleased, key) != keysReleased.end();
}
bool Input::isKeyDown(SDL_Keycode key)
{
	return std::ranges::find(keysCurrentlyDown, key) != keysCurrentlyDown.end();
}

bool Input::wasMouseButtonPressed(Uint8 button)
{
	return std::ranges::find(mouseButtonsPressed, button) != mouseButtonsPressed.end();
}
bool Input::wasMouseButtonReleased(Uint8 button)
{
	return std::ranges::find(mouseButtonsReleased, button) != mouseButtonsReleased.end();
}
bool Input::isMouseButtonDown(Uint8 button)
{
	return std::ranges::find(mouseButtonsCurrentlyDown, button) != mouseButtonsCurrentlyDown.end();
}
