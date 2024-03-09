#pragma once
#include "glm/glm.hpp"

class Texture;

class Renderer
{
public:
	static void renderTex(const Texture* tex, glm::ivec2 pos, glm::ivec2 size);
};
