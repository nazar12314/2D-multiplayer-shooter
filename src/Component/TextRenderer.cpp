#include "TextRenderer.h"

#include "Assets.h"
#include "Material.h"

TextRenderer::TextRenderer(GameObject* obj, const std::string& text, glm::vec2 size, const Color& color, int order, Font* font) : BaseRenderer(obj, size, color, order),
	_text(text), _font(font)
{
	if (_font == nullptr)
		_font = Assets::load<Font>("assets/fonts/visitor.ttf", 24);

	_material = new Material(Texture(_font, _text, _color));
	_preserveAspect = true;
}
void TextRenderer::updateTexture() const
{
	_material->setTexture(Texture(_font, _text, _color));
}

void TextRenderer::setText(const std::string& text)
{
	_text = text;
	updateTexture();
}
void TextRenderer::setColor(const Color& color)
{
	_color = color;
	updateTexture();
}
void TextRenderer::setFont(Font* font)
{
	_font = font;
	updateTexture();
}
