#include "Application.h"

#include "MyTime.h"
#include "Object.h"
#include "Physics.h"
#include "Renderer.h"
#include "SDLHandler.h"
#include "FPSCounter.h"
#include "Scene.h"
#include "DOVirtual.h"

void Application::start(const glm::ivec2& windowSize)
{
	srand(time(NULL));

	SDLHandler::init(windowSize);
	Renderer::init();
	Time::init();
	Physics::init();

	Scene::create();
	Object::sendCallbackAll(&Component::start);
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

		DOVirtual::update(Time::deltaTime);
		Object::sendCallbackAll(&Component::update);
		Object::sendCallbackAll(&Component::lateUpdate);

		if (doQuit) break;
		Renderer::render();

	}
}

void Application::quit()
{
	Object::destroyAll();

	SDLHandler::uninit();
}
