#pragma once

#include <memory>
#include <vector>

class GameObject;
using GameObjectSPtr = std::shared_ptr<GameObject>;

class Object
{
	// -- Global --
	inline static std::vector<Object*> _toDestroy;

	static void prepare();

public:
	/* Destroys the object after the current frame */
	static void destroy(Object* obj);

	/* Destroys the object after the current frame */
	static void destroy(const GameObjectSPtr& obj);
	// -- Global --

protected:
	bool _isDestroyed = false;

	Object() = default;
	virtual ~Object() = default;

	virtual void preDestroyInternal() {}
	virtual void destroyImmediate();
	void throwIfDestroyed() const;

public:
	bool isDestroyed() const;

	friend class Application;
	friend class GameObject;


	// DOTween
	void doKill() const;
};
