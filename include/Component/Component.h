#pragma once

class Object;

class Component
{
public:
	Object* obj;

	Component(Object* obj);
	virtual ~Component();

	virtual void start() {}
	virtual void update() {}
	virtual void destroy() {}
};
