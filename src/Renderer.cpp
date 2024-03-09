#include "Renderer.h"

#include "Object.h"
#include "Application.h"
#include "Texture.h"

void Renderer::renderTex(const Texture* tex, glm::ivec2 pos, glm::ivec2 size)
{
	SDL_Rect rect;
	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = size.x;
	rect.h = size.y;

	SDL_RenderCopy(Application::renderer, tex->texture, NULL, &rect);
}