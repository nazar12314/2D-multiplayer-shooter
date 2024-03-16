#include "Renderer.h"

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
		else if (auto camera = dynamic_cast<Camera*>(comp))
			cameras.push_back(camera);
	};
	Object::onComponentRemoved += [](Component* comp)
	{
		if (auto sprite = dynamic_cast<Sprite*>(comp))
			std::erase(sprites, sprite);
		else if (auto camera = dynamic_cast<Camera*>(comp))
			std::erase(cameras, camera);
	};
}

void Renderer::render()
{
	if (cameras.empty()) return;

	// Find camera with highest priority
	auto lambda = [](const Camera* a, const Camera* b) { return a->priority < b->priority; };
	auto targetCamera = *std::max_element(cameras.begin(), cameras.end(), lambda);

	SDL_RenderClear(SDLHandler::renderer);
	for (auto sprite : sprites)
	{
		if (!sprite->obj->enabled) continue;
		sprite->draw(targetCamera->obj->pos(), targetCamera->size);
	}
	SDL_RenderPresent(SDLHandler::renderer);
}

void Renderer::renderTex(const Texture* tex, const glm::ivec2& pos, const glm::ivec2& size)
{
	SDL_Rect rect;
	rect.x = pos.x - size.x / 2;
	rect.y = pos.y - size.y / 2;
	rect.w = size.x;
	rect.h = size.y;

	SDL_RenderCopy(SDLHandler::renderer, tex->texture, NULL, &rect);
}
void Renderer::renderTexRotated(const Texture* tex, const glm::ivec2& pos, const glm::ivec2& size, float rot)
{
	SDL_Rect rect;
	rect.x = pos.x - size.x / 2;
	rect.y = pos.y - size.y / 2;
	rect.w = size.x;
	rect.h = size.y;

	SDL_RenderCopyEx(SDLHandler::renderer, tex->texture, NULL, &rect, rot, NULL, SDL_FLIP_NONE);
}
void Renderer::renderTexNormalized(const Texture* tex, const glm::vec2& pos, const glm::vec2& size, float rot)
{
	auto wSize = (glm::vec2)SDLHandler::windowSize;
	auto ratio = (float)SDLHandler::windowSize.x / SDLHandler::windowSize.y;

	auto screenPos = glm::vec2(pos.x + ratio / 2, 0.5f - pos.y) * wSize.y; // y-axis is inverted
	auto screenSize = size * wSize.y;
	renderTexRotated(tex, screenPos, screenSize, rot);
}
