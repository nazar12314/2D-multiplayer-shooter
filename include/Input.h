#pragma once

#include <SDL_events.h>

class Input
{
public:
	static void updateInput();
	static void handleSDLEvent(const SDL_Event& event);
};