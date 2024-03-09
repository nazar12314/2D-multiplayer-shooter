#pragma once

#include <SDL_events.h>

class Input
{
public:
	static void updateInput();

public:
	static void handleInputEvent(const SDL_Event& event);
};
