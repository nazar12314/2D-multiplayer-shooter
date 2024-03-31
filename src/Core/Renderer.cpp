#include "Renderer.h"

#include "Object.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "SDLHandler.h"
#include "Assets.h"

void Renderer::init()
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

	SDL_RenderClear(SDLHandler::renderer);
	for (auto sprite : sprites)
	{
		if (!sprite->obj->enabled()) continue;
		auto screenPos = mainCamera->worldToScreenPos(sprite->obj->pos());
		auto screenSize = sprite->size() / (glm::vec2)mainCamera->size() * (float)SDLHandler::windowSize.y;
		renderTex(sprite->sprite(), screenPos, screenSize, sprite->obj->rot());
	}
	SDL_RenderPresent(SDLHandler::renderer);
}

void Renderer::renderTex(const Texture* tex, const glm::ivec2& pos, const glm::ivec2& size, float rot)
{
	SDL_Rect rect;
	rect.x = pos.x - size.x / 2;
	rect.y = pos.y - size.y / 2;
	rect.w = size.x;
	rect.h = size.y;

	SDL_RenderCopyEx(SDLHandler::renderer, tex->texture(), NULL, &rect, rot, NULL, SDL_FLIP_NONE);
}
void Renderer::sortSprites()
{
	std::sort(sprites.begin(), sprites.end(), [](const SpriteRenderer* a, const SpriteRenderer* b) { return a->_order < b->_order; });
}
