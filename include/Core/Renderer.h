#pragma once
#include <vector>

#include "glm/glm.hpp"

class Camera;
class Graphical;
class Texture;

class Renderer
{
	inline static std::vector<Camera*> cameras{};
	inline static std::vector<Graphical*> graphicals{};

public:
	static void init();

	static void render();

	static void renderTex(const Texture* tex, const glm::ivec2& pos, const glm::ivec2& size);
	static void renderTexNormalized(const Texture* tex, const glm::vec2& pos, const glm::vec2& size);
};
