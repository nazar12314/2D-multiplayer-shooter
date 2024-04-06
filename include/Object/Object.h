#pragma once
#include <vector>

class Object
{
	// -- Global --
	inline static std::vector<Object*> _toDestroy;

	static void prepare();

public:
	/* Destroys the object after the current frame */
	static void destroy(Object* obj);
	// -- Global --

protected:
	bool _isDestroyed = false;

	Object() = default;
	virtual ~Object() = default;

	virtual void destroyInternal() {}
	virtual void destroyImmediate();
	void throwIfDestroyed() const;

public:
	bool isDestroyed() const;

	friend class Application;
};
