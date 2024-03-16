#pragma once
#include <vector>

#include "glm/glm.hpp"

class Camera;
class Sprite;
class Texture;

class Renderer
{
	inline static std::vector<Camera*> cameras{};
	inline static std::vector<Sprite*> sprites{};

public:
	static void init();

	static void render();

	static void renderTex(const Texture* tex, const glm::ivec2& pos, const glm::ivec2& size);
	static void renderTexRotated(const Texture* tex, const glm::ivec2& pos, const glm::ivec2& size, float rot);
	static void renderTexNormalized(const Texture* tex, const glm::vec2& pos, const glm::vec2& size, float rot = 0);
};
