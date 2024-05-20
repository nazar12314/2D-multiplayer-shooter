#pragma once

#include <memory>
#include <SDL_events.h>

#include "Action.h"
#include "Utils.h"
#include "glm/vec2.hpp"

class Collider;

class Input
{
	inline static std::vector<SDL_Keycode> keysPressed {};
	inline static std::vector<SDL_Keycode> keysReleased {};
	inline static std::vector<SDL_Keycode> keysCurrentlyDown {};

	inline static std::vector<Uint8> mouseButtonsPressed {};
	inline static std::vector<Uint8> mouseButtonsReleased {};
	inline static std::vector<Uint8> mouseButtonsCurrentlyDown {};

	inline static SPtr<Collider> mouseOverCollider = nullptr;
	inline static SPtr<Collider> mouseDownCollider = nullptr;

	inline static glm::ivec2 _mousePos {};
	inline static float _mouseWheelValue = 0;

	static void clear();
	static void handleInputEvent(const SDL_Event& event);

public:
	inline static Action<SDL_Event> onEvent {};

	// SDLK_{X} - X key, e.g. SDLK_w - W key
	static bool wasKeyPressed(SDL_Keycode key);
	static bool wasKeyReleased(SDL_Keycode key);
	static bool isKeyDown(SDL_Keycode key);

	// SDL_BUTTON_LEFT - left click, SDL_BUTTON_RIGHT - right click
	static bool wasMouseButtonPressed(Uint8 button);
	static bool wasMouseButtonReleased(Uint8 button);
	static bool isMouseButtonDown(Uint8 button);

	static const glm::ivec2& mousePos();
	static float mouseWheelValue();

	friend class SDLHandler;
};
