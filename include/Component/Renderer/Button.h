#pragma once

#include "TextRenderer.h"

class Button : public TextRenderer
{
	Color bgColor = Color::WHITE;
	Texture* bgTexture = nullptr;
	float borderSize = 0.1f;

	Button(GameObject* obj, const std::string& text = "", const glm::vec2& size = { 1, 1 }, const Color& textColor = Color::WHITE, int order = 0, Font* font = nullptr);

	void render(const Camera* camera) const override;

	void onMouseClick() override;

public:
	Action<> onClick {};

	void setBgColor(const Color& color);

	friend class GameObject;
};
