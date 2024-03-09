#include "Renderer.h"

#include "Object.h"
#include "Application.h"
#include "Texture.h"

void Renderer::renderTex(const Texture* tex, const glm::ivec2& pos, const glm::ivec2& size)
{
	SDL_Rect rect;
	rect.x = pos.x - size.x / 2;
	rect.y = pos.y - size.y / 2;
	rect.w = size.x;
	rect.h = size.y;

	SDL_RenderCopy(Application::renderer, tex->texture, NULL, &rect);
}
void Renderer::renderTexNormalized(const Texture* tex, const glm::vec2& pos, const glm::vec2& size)
{
	auto wSize = ((glm::vec2)Application::windowSize);
	auto ratio = (float)Application::windowSize.x / Application::windowSize.y;

	auto screenPos = glm::vec2(pos.x + ratio / 2, 0.5f - pos.y) * wSize.y; // y-axis is inverted
	auto screenSize = size * wSize.y;
	renderTex(tex, screenPos, screenSize);
}
