#include "InputField.h"

#include "Assets.h"
#include "Input.h"
#include "PolygonCollider.h"
#include "Texture.h"
#include "Transform.h"

InputField::InputField(GameObject* obj, const std::string& text, const glm::vec2& size, const Color& textColor, int order, Font* font):
	TextRenderer(obj, text, size, textColor, order, font), placeholder(text)
{
	bgTexture = new Texture(Assets::load<Sprite>("assets/sprites/square.png"), bgColor);

	onEventSub = Input::onEvent.subscribe([this](const SDL_Event& event) { onEvent(event); });

	addComponent<BoxCollider>(size, true);
}

void InputField::render(const Camera* camera) const
{
	auto pos = transform()->pos();
	auto size = _size + glm::vec2(borderSize * 2);
	float rot = transform()->rot();
	Renderer::renderTexWorld(bgTexture->texture(), pos, size, rot, camera);

	RectRenderer::render(camera);
}

void InputField::onEvent(const SDL_Event& event)
{
	if (!focused) return;

	auto text = getText();
	if (event.type == SDL_TEXTINPUT)
	{
		setText(text + event.text.text);
	}
	else if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym != SDLK_BACKSPACE || text.empty()) return;
		setText(text.substr(0, text.size() - 1));
	}
}

void InputField::update()
{
	if (focused && !hovered && Input::wasMouseButtonPressed(SDL_BUTTON_LEFT))
	{
		focused = false;
		SDL_StopTextInput();

		bgColor = Color::WHITE;
		bgTexture->setColor(bgColor);

		if (getText().empty())
		{
			showingPlaceholder = true;
			setText(placeholder);
		}
	}
}

void InputField::onMouseEnter()
{
	hovered = true;
}
void InputField::onMouseExit()
{
	hovered = false;
}
void InputField::onMouseClick()
{
	focused = true;
	SDL_StartTextInput();

	bgColor = Color(0.88f, 0.88f, 0.88f);
	bgTexture->setColor(bgColor);

	if (showingPlaceholder)
	{
		showingPlaceholder = false;
		setText("");
	}
}

void InputField::onDestroy()
{
	Input::onEvent.unsubscribe(onEventSub);
	delete bgTexture;
}
