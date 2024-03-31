#pragma once
#include <vector>

#include "glm/glm.hpp"

class Camera;
class SpriteRenderer;
class Texture;

class Renderer
{
	inline static std::vector<SpriteRenderer*> sprites{};

public:
	static void init();

	static void render();
	static void renderTex(const Texture* tex, const glm::ivec2& pos, const glm::ivec2& size, float rot = 0);

	static void sortSprites();
};
