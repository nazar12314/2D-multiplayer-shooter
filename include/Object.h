// ReSharper disable CppMemberFunctionMayBeStatic
#pragma once

#include <vector>

#include "Action.h"
#include "Transform.h"

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
