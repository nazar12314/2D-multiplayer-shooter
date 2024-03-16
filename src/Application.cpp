#include "Application.h"

#include "BoxCollider.h"
#include "MyTime.h"
#include "Object.h"
#include "Renderer.h"
#include "SDLHandler.h"

#include "Scene.h"

void Application::start(const glm::ivec2& windowSize)
{
	SDLHandler::init(windowSize);
	Renderer::init();
	Scene::init();

	Object::startAll();
}

void Application::loop()
{
	while (true)
	{
		SDLHandler::handleEvents();
		Time::tick();
		Object::updateAll();

		if (doQuit) break;
		Renderer::render();
	}
}

void Application::quit()
{
	Object::destroyAll();

	SDLHandler::uninit();
}

