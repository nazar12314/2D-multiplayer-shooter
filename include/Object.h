// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

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

	Object(glm::vec2 pos = {0, 0}, float rot = 0);
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
	Graphical(Texture* texture, glm::ivec2 size = {1, 1}, const glm::vec2& pos = {0, 0}, float rot = 0);

	void draw(const glm::ivec2& cameraPos, int cameraSize) const;

	void setNativeSize();
};
