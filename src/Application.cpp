#include "Application.h"

#include "MyTime.h"
#include "Object.h"
#include "Physics.h"
#include "Renderer.h"
#include "SDLHandler.h"
#include "FPSCounter.h"
#include "Scene.h"

void Application::start(const glm::ivec2& windowSize)
{
	SDLHandler::init(windowSize);
	Renderer::init();
	Time::init();
	Physics::init();

	Scene::create();
	Object::startAll();
}

void Application::loop()
{
	while (true)
	{
		SDLHandler::handleEvents();
		Object::prepareAll();
		Physics::physicsLoop();
		Time::tick();
		FPSCounter::tick();

		Object::updateAll();
		Object::lateUpdateAll();

		if (doQuit) break;
		Renderer::render();
	}
}

void Application::quit()
{
	Object::destroyAll();

	SDLHandler::uninit();
}
