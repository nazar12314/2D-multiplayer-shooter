#include "Input.h"

#include "Application.h"
#include "Camera.h"
#include "Physics.h"
#include <algorithm>

void Input::clear()
{
	keysPressed.clear();
	keysReleased.clear();
	mouseButtonsPressed.clear();
	mouseButtonsReleased.clear();
	_mouseWheelValue = 0;
}

void Input::handleInputEvent(const SDL_Event& event)
{
	if (!SDL_IsTextInputActive())
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
			std::erase(keysCurrentlyDown, event.key.keysym.sym);
		}
	}
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		mouseButtonsPressed.push_back(event.button.button);
		mouseButtonsCurrentlyDown.push_back(event.button.button);

		auto mouseWorldPos = Camera::getMain()->screenToWorldPoint(_mousePos);
		mouseDownCollider = Physics::raycastAt(mouseWorldPos);
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		mouseButtonsReleased.push_back(event.button.button);
		std::erase(mouseButtonsCurrentlyDown, event.button.button);

		auto mouseWorldPos = Camera::getMain()->screenToWorldPoint(_mousePos);
		auto hitCollider = Physics::raycastAt(mouseWorldPos);
		if (hitCollider == nullptr || hitCollider != mouseDownCollider) return;

		hitCollider->gameObject()->sendCallback(&Component::onMouseClick);
	}
	else if (event.type == SDL_MOUSEMOTION)
	{
		_mousePos.x = event.motion.x;
		_mousePos.y = event.motion.y;

		auto mouseWorldPos = Camera::getMain()->screenToWorldPoint(_mousePos);
		auto hitCollider = Physics::raycastAt(mouseWorldPos);
		if (hitCollider == mouseOverCollider) return;

		if (mouseOverCollider != nullptr && !mouseOverCollider->isDestroyed())
			mouseOverCollider->gameObject()->sendCallback(&Component::onMouseExit);
		mouseOverCollider = hitCollider;

		if (hitCollider == nullptr) return;
		hitCollider->gameObject()->sendCallback(&Component::onMouseEnter);
	}
	else if (event.type == SDL_MOUSEWHEEL)
	{
		_mouseWheelValue = event.wheel.y;
	}

	onEvent(event);
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

const glm::ivec2& Input::mousePos() { return _mousePos; }
float Input::mouseWheelValue() { return _mouseWheelValue; }
