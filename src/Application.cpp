#include "Application.h"

#include "BoxCollider.h"
#include "Camera.h"
#include "CameraMover.h"
#include "Graphical.h"
#include "Input.h"
#include "MyTime.h"
#include "Object.h"
#include "Renderer.h"
#include "SDLHandler.h"
#include "Texture.h"

#include <iostream>

#include "CircleCollider.h"

void Application::init(const glm::ivec2& windowSize)
{
	Application::windowSize = windowSize;

	initSDL(windowSize);
	initScene();
}
void Application::initSDL(const glm::ivec2& windowSize)
{
	window = SDL_CreateWindow("2D Multiplayer Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowSize.x, windowSize.y, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	surface = SDL_GetWindowSurface(window);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Application::initScene()
{
	auto cam = new Camera(600);
	auto mover = cam->addComponent<CameraMover>();
	mover->speed = 5;

	auto tex1 = new Texture("sprites/square.png");
	auto tex2 = new Texture("sprites/circle.png");

	int x1 = 10, y1 = 10, height1 = 100, width1 = 200;
	int x2 = 110, y2 = 10, height2 = 100, width2 = 200;

	auto obj1 = new Graphical(tex1, {width1, height1}, {x1, y1});
	auto obj2 = new Graphical(tex2, {height2, height2}, {x2, y2});

	auto box1 = obj1->addComponent<BoxCollider>();
	box1->size = {width1, height1};
	obj1->setRot(10);

	auto circle1 = obj2->addComponent<CircleCollider>();
	circle1->radius = 50;
	std::cout << "collides: " << box1->collidesWith(circle1) << std::endl;
}

void Application::loop()
{
	Object::startAll();

	while (true)
	{
		SDLHandler::handleEvents();
		Time::tick();
		Object::updateAll();

		if (doQuit) break;

		SDL_RenderClear(renderer);
		Camera::renderMain();
		SDL_RenderPresent(renderer);
	}

	Object::destroyAll();
}

void Application::quit()
{
	uninitSDL();
}
void Application::uninitSDL()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
