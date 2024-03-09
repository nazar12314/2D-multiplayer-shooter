#include "Renderer.h"

#include "Object.h"
#include "SDLHandler.h"
#include "Texture.h"

void Renderer::draw()
{
	for (auto obj : Object::objects)
	{
		if (!obj->enabled)continue;
		auto graphical = dynamic_cast<Graphical*>(obj);
		if (graphical == nullptr) continue;
		graphical->draw();
	}
}
void Renderer::drawTex(const Texture* tex, int x, int y, int w, int h)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_RenderCopy(SDLHandler::renderer, tex->texture, NULL, &rect);
}
void Renderer::drawTex(const Texture* tex, glm::ivec2 pos, glm::ivec2 size)
{
	SDL_Rect rect;
	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = size.x;
	rect.h = size.y;

	SDL_RenderCopy(SDLHandler::renderer, tex->texture, NULL, &rect);
}
