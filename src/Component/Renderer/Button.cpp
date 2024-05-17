#include "Button.h"

#include "Assets.h"
#include "Input.h"
#include "PolygonCollider.h"
#include "Texture.h"
#include "Transform.h"

Button::Button(GameObject* obj, const std::string& text, const glm::vec2& size, const Color& textColor, int order, Font* font) :
	TextRenderer(obj, text, size, textColor, order, font)
{
	bgTexture = new Texture(Assets::load<Sprite>("assets/sprites/square.png"), bgColor);

	addComponent<BoxCollider>(size, true);
}

void Button::render(const Camera* camera) const
{
	auto pos = transform()->pos();
	auto size = _size + glm::vec2(borderSize * 2);
	float rot = transform()->rot();
	Renderer::renderTexWorld(bgTexture->texture(), pos, size, rot, camera);

	RectRenderer::render(camera);
}
void Button::onMouseClick()
{
	onClick();
}

void Button::setBgColor(const Color& color)
{
	bgColor = color;
	bgTexture->setColor(bgColor);
}
