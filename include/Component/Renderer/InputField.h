#pragma once

#include "SDL_events.h"
#include "TextRenderer.h"

class Font;

class InputField : public TextRenderer
{
	Color bgColor = Color::WHITE;
	Texture* bgTexture = nullptr;
	float borderSize = 0.1f;

	std::string placeholder;
	bool showingPlaceholder = true;

	bool hovered = false;
	bool focused = false;

	InputField(GameObject* obj, const std::string& text = "",const glm::vec2& size = {1, 1}, const Color& textColor = Color::WHITE, int order = 0, Font* font = nullptr);

	void render(const Camera* camera) const override;

	int onEventSub;
	void onEvent(const SDL_Event& event);
	void update() override;

	void onMouseEnter() override;
	void onMouseExit() override;
	void onMouseClick() override;

	void onDestroy() override;

	friend class GameObject;
};
