#pragma once

#include <string>

#include "BaseRenderer.h"
#include "Color.h"

class Font;

class TextRenderer : public RectRenderer
{
	std::string _text;
	Font* _font;

protected:
	TextRenderer(GameObject* obj, const std::string& text = "", const glm::vec2& size = {1, 1}, const Color& color = Color::WHITE, int order = 0, Font* font = nullptr);

	void updateTexture();

public:
	std::string getText() const { return _text; }
	const Color& getColor() const { return _color; }
	Font* getFont() const { return _font; }

	void setText(const std::string& text);
	void setColor(const Color& color) override;
	void setFont(Font* font);

	friend class GameObject;
};
