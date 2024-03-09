#pragma once
#include "glm/glm.hpp"

class Texture;

class Renderer
{
public:
	static void draw();

	static void drawTex(const Texture* tex, int x, int y, int w, int h);
	static void drawTex(const Texture* tex, glm::ivec2 pos, glm::ivec2 size);
};
