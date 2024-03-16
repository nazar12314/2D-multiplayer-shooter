#include "Renderer.h"

#include <iostream>

#include "Object.h"
#include "Application.h"
#include "Camera.h"
#include "Sprite.h"
#include "SDLHandler.h"
#include "Texture.h"

void Renderer::init()
{
	Object::onComponentAdded += [](Component* comp)
	{
		if (auto sprite = dynamic_cast<Sprite*>(comp))
			sprites.push_back(sprite);
	};
	Object::onComponentRemoved += [](Component* comp)
	{
		if (auto sprite = dynamic_cast<Sprite*>(comp))
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
		if (!sprite->obj->enabled) continue;

		auto screenPos = mainCamera->worldToScreenPos(sprite->obj->pos());
		auto screenSize = (glm::vec2)sprite->size() / (glm::vec2)mainCamera->size() * (float)SDLHandler::windowSize.y;
		renderTex(sprite->texture(), screenPos, screenSize, sprite->obj->rot());
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

	SDL_RenderCopyEx(SDLHandler::renderer, tex->texture, NULL, &rect, rot, NULL, SDL_FLIP_NONE);
}