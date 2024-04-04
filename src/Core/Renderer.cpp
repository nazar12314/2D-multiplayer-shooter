#include "Renderer.h"

#include "Object.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "SDLHandler.h"
#include "Assets.h"
#include "Gizmos.h"
#include "Material.h"
#include <algorithm>

Material* Renderer::defaultCircleMaterial = new Material(Assets::load<Texture>("sprites/circle.png"));

void Renderer::init()
{
	subscribeToEvents();
}
void Renderer::subscribeToEvents()
{
	Object::onComponentAddedGlobal += [](Component* comp)
	{
		if (auto sprite = dynamic_cast<SpriteRenderer*>(comp))
		{
			sprites.push_back(sprite);
			sortSprites();
		}
	};
	Object::onComponentRemovedGlobal += [](Component* comp)
	{
		if (auto sprite = dynamic_cast<SpriteRenderer*>(comp))
			std::erase(sprites, sprite);
	};
}

void Renderer::render()
{
	auto mainCamera = Camera::getMain();
	if (mainCamera == nullptr) return;

	SDL_SetRenderDrawColor(SDLHandler::renderer, 0, 0, 0, 255);
	SDL_RenderClear(SDLHandler::renderer);
	renderSprites(mainCamera);
	Gizmos::draw();
	SDL_RenderPresent(SDLHandler::renderer);
}
void Renderer::renderSprites(const Camera* mainCamera)
{
	for (auto sprite : sprites)
	{
		if (!sprite->obj->enabled()) continue;
		auto screenPos = mainCamera->worldToScreenPoint(sprite->obj->pos());
		auto screenSize = sprite->size() / (glm::vec2)mainCamera->size() * (float)SDLHandler::windowSize.y;
		renderTex(sprite->material()->texture(), screenPos, screenSize, sprite->obj->rot());
	}
}

void Renderer::renderTex(SDL_Texture* tex, const glm::ivec2& pos, const glm::ivec2& size, float rot)
{
	SDL_Rect rect;
	rect.x = pos.x - size.x / 2;
	rect.y = pos.y - size.y / 2;
	rect.w = size.x;
	rect.h = size.y;

	SDL_RenderCopyEx(SDLHandler::renderer, tex, NULL, &rect, rot, NULL, SDL_FLIP_NONE);
}
void Renderer::renderTexWorld(SDL_Texture* tex, const glm::vec2& pos, const glm::vec2& size, float rot)
{
	auto screenPos = Camera::getMain()->worldToScreenPoint(pos);
	auto screenSize = Camera::getMain()->worldToScreenSize(size);
	renderTex(tex, screenPos, screenSize, rot);
}

void Renderer::drawLine(const glm::vec2& p1, const glm::vec2& p2, const Color& color)
{
	auto screenP1 = Camera::getMain()->worldToScreenPoint(p1);
	auto screenP2 = Camera::getMain()->worldToScreenPoint(p2);

	SDL_SetRenderDrawColor(SDLHandler::renderer, color.r() * 255, color.g() * 255, color.b() * 255, color.a() * 255);
	SDL_RenderDrawLine(SDLHandler::renderer, screenP1.x, screenP1.y, screenP2.x, screenP2.y);
}
void Renderer::drawCircle(const glm::vec2& pos, float radius, const Color& color)
{
	defaultCircleMaterial->setColor(color);
	renderTexWorld(defaultCircleMaterial->texture(), pos, {radius * 2, radius * 2});
}
void Renderer::sortSprites()
{
	std::sort(sprites.begin(), sprites.end(), [](const SpriteRenderer* a, const SpriteRenderer* b) { return a->_order < b->_order; });
}
