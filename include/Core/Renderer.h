#pragma once
#include "glm/glm.hpp"

class Texture;

class Renderer
{
public:
	static void renderTex(const Texture* tex, const glm::ivec2& pos, const glm::ivec2& size);
	static void renderTexNormalized(const Texture* tex, const glm::vec2& pos, const glm::vec2& size);
};
