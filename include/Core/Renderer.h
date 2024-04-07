#pragma once
#include <SDL_render.h>
#include <vector>

#include "Color.h"
#include "glm/glm.hpp"

class Texture;
class BaseRenderer;
class Camera;

class Renderer
{
	inline static std::vector<BaseRenderer*> renderers {};

	inline static Texture* circleTex = nullptr;
	inline static Texture* squareTex = nullptr;

	static void drawLine(const glm::vec2& p1, const glm::vec2& p2, const Color& color, float width = 3);
	static void drawCircleWorld(const glm::vec2& pos, float radius, const Color& color);

public:
	static void init();
	static void subscribeToEvents();

	static void render();
	static void renderClear();
	static void renderObjects(const Camera* mainCamera);

	static void renderTex(SDL_Texture* tex, const glm::vec2& pos, const glm::vec2& size, float rot = 0);
	static void renderTexWorld(SDL_Texture* tex, const glm::vec2& pos, const glm::vec2& size, float rot = 0);

	static void sortRenderers();

	friend class PolygonGizmo;
	friend class PointGizmo;
	friend class VectorGizmo;
};
