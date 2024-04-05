#include "Renderer.h"

#include "Object.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "SDLHandler.h"
#include "Assets.h"
#include "Gizmos.h"
#include "Material.h"
#include <algorithm>
#include <iostream>

#include "Transform.h"
#include "glm/gtx/string_cast.hpp"

void Renderer::init()
{
	subscribeToEvents();

	circleMaterial = new Material(Assets::load<Texture>("sprites/circle.png"));
	squareMaterial = new Material(Assets::load<Texture>("sprites/square.png"));
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
		if (!sprite->obj()->enabled()) continue;
		auto screenPos = mainCamera->worldToScreenPoint(sprite->transform()->getPos());
		auto screenSize = sprite->size() / (glm::vec2)mainCamera->size() * (float)SDLHandler::windowSize.y;
		renderTex(sprite->material()->texture(), screenPos, screenSize, sprite->transform()->getRot() - mainCamera->transform()->getRot());
	}
}

void Renderer::renderTex(SDL_Texture* tex, const glm::vec2& pos, const glm::vec2& size, float rot)
{
	SDL_FRect rect;
	rect.x = pos.x - size.x / 2;
	rect.y = pos.y - size.y / 2;
	rect.w = size.x;
	rect.h = size.y;

	SDL_RenderCopyExF(SDLHandler::renderer, tex, NULL, &rect, -rot, NULL, SDL_FLIP_NONE);
}
void Renderer::renderTexWorld(SDL_Texture* tex, const glm::vec2& pos, const glm::vec2& size, float rot)
{
	auto screenPos = Camera::getMain()->worldToScreenPoint(pos);
	auto screenSize = Camera::getMain()->worldToScreenSize(size);
	renderTex(tex, screenPos, screenSize, rot);
}

void Renderer::drawLine(const glm::vec2& p1, const glm::vec2& p2, const Color& color, float width)
{
	squareMaterial->setColor(color);
	renderTexWorld(squareMaterial->texture(), (p1 + p2) / 2.f, {width, distance(p1, p2)}, 90 - glm::degrees(glm::atan(p2.y - p1.y, p2.x - p1.x)));
}
void Renderer::drawCircleWorld(const glm::vec2& pos, float radius, const Color& color)
{
	circleMaterial->setColor(color);
	renderTexWorld(circleMaterial->texture(), pos, {radius * 2, radius * 2});
}
void Renderer::sortSprites()
{
	std::ranges::sort(sprites, [](const SpriteRenderer* a, const SpriteRenderer* b) { return a->_order < b->_order; });
}
