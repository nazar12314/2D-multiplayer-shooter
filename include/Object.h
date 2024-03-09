// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <SDL_render.h>
#include <vector>

#include "Action.h"
#include "Transform.h"

class Texture;

class Object : public Transform
{
public:
	// -- Global --
	inline static std::vector<Object*> objects {};

	inline static Action<Object*> onObjectAdded {};
	inline static Action<Object*> onObjectRemoved {};

	static void addObject(Object* obj);
	static void removeObject(Object* obj);

	static void startAll();
	static void updateAll();
	static void destroyAll();
	// -- Global --

public:
	bool enabled;

	Object(glm::vec2 pos, float rot = 0);
	virtual ~Object();

	void start() {}
	void update() {}
	void onDestroy() {}
};


class Graphical : public Object
{
	Texture* texture;
	glm::ivec2 size;

public:
	Graphical(const glm::vec2& pos, float rot, Texture* texture, glm::ivec2 size = {1, 1});

	void draw() const;

	void setNativeSize();
};
