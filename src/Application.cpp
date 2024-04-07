#include "Application.h"

#include "MyTime.h"
#include "GameObject.h"
#include "Physics.h"
#include "Renderer.h"
#include "SDLHandler.h"
#include "FPSCounter.h"
#include "Scene.h"
#include "DOVirtual.h"

void Application::start(const glm::ivec2& windowSize)
{
	SDLHandler::init(windowSize);
	Renderer::init();
	Time::init();
	Physics::init();

	Scene::create();
}

void Application::loop()
{
	while (true)
	{
		Object::prepare();
		SDLHandler::handleEvents();
		Physics::physicsLoop();
		Time::tick();
		FPSCounter::tick();

		GameObject::sendCallbackAll(&Component::update);
		DOVirtual::update(Time::deltaTime);
		GameObject::sendCallbackAll(&Component::lateUpdate);

		if (doQuit) break;
		Renderer::render();
	}
}

void Application::quit()
{
	GameObject::destroyAll();

	SDLHandler::uninit();
}
