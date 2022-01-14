#include "Game.h"
#include "Timer.h"
#include "Assets.h"
#include "Actor.h"
#include "Astro�d.h"
#include "SpriteComponent.h"
#include "AnimSpriteComponent.h"
#include "BackgroundSpriteComponent.h"
#include "Ship.h"


#include <vector>

using std::vector;

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);

	/*int windowWidth = window.getWidth();
	int windowHeight = window.getHeight();*/

	return isWindowInit && isRendererInit; // Return bool && bool && bool ...to detect error
}

void Game::load() {
	Assets::loadTexture(renderer, "Resources\\Farback01.png", "Farback01");
	Assets::loadTexture(renderer, "Resources\\Farback02.png", "Farback02");
	Assets::loadTexture(renderer, "Resources\\Stars.png", "Stars");
	Assets::loadTexture(renderer, "Resources\\Astroid.png", "Astroid");
	Assets::loadTexture(renderer, "Resources\\Ship.png", "Ship");
	Assets::loadTexture(renderer, "Resources\\Laser.png", "Laser");
	

	// Controlled ship
	Ship* ship = new Ship();
	ship->setPosition(Vector2{ 100, 300 });


	// Background
	// Create the farest background
	vector<Texture*> bgTexsFar {
		&Assets::getTexture("Farback01"),
		&Assets::getTexture("Farback02")
	};

	Actor* bgFar = new Actor();
	BackgroundSpriteComponent* bgSpritesFar = new BackgroundSpriteComponent(bgFar, bgTexsFar);
	bgSpritesFar->setScrollSpeed(-100.0f);

	// Create the closer background
	Actor* bgClose = new Actor();
	std::vector<Texture*> bgTexsClose {
		&Assets::getTexture("Stars"),
		&Assets::getTexture("Stars")
	};
	BackgroundSpriteComponent* bgSpritesClose = new BackgroundSpriteComponent(bgClose, bgTexsClose, 50);
	bgSpritesClose->setScrollSpeed(-200.0f);

	const int astroidNumber = 20;
	for (int i = 0; i < astroidNumber; ++i) {
		new Astroid();
	}

}



void Game::processInput()
{
	// SDL Event
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}

	// Keyboard state
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	// Escape: quit game
	if (keyboardState[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}

	// Actor input
	isUpdatingActors = true;

	for (auto actor : actors) {
		actor->processInput(keyboardState);
	}
	isUpdatingActors = false;
}

void Game::update(float dt) {

	//Update Actors
	isUpdatingActors = true;
	for (auto actor : actors) {

		actor->update(dt);
	}

	isUpdatingActors = false;

	//Move pending actors to actors
	for (auto pendingActor : pendingActors) {
		actors.emplace_back(pendingActor);
	}
	pendingActors.clear();

	//Delete dead actors
	vector<Actor*> deadActors;
	for (auto actor : actors) {
		if (actor->getState() == Actor::ActorState::Dead) {
			deadActors.emplace_back(actor);
		}
	}

	for (auto deadActor : deadActors) {
		delete deadActor;
	}
}

void Game::addActor(Actor* actor) {
	if (isUpdatingActors) {
		pendingActors.emplace_back(actor);
	}
	else {
		actors.emplace_back(actor);

	}
}

void Game::removeActor(Actor* actor) {
	//Erase actor from the two vectors 
	auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
	if (iter != end(pendingActors)) {
		//Swap to end of vector and pop off (avoid erase copies
		std::iter_swap(iter, end(pendingActors) - 1);
		pendingActors.pop_back();
	}
	iter = std::find(begin(actors), end(actors), actor);
	if (iter != end(actors)) {
		std::iter_swap(iter, end(actors) - 1);
		actors.pop_back();
	}
}

vector<Astroid*>& Game::getAstroids() {
	return astroids;
}

void Game::addAstroid(Astroid * astroid){
	astroids.emplace_back(astroid);
}

void Game::removeAstroid(Astroid* astroid) {
	auto iter = std::find(begin(astroids), end(astroids), astroid);

	if (iter != astroids.end()) {
		astroids.erase(iter);
	}
}

void Game::render() {
	renderer.beginDraw();
	renderer.draw();
	renderer.endDraw();
}

void Game::loop() {
	Timer timer;
	float dt = 0;
	while (isRunning) {
		float dt = timer.computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		render();
		timer.delayTime();
	}
}

void Game::unload() {
	//Delete actors
	//Because ~Actor calls RemoveActor, have to use different style loop
	while (!actors.empty()) {
		delete actors.back();
	}

	//Resources
	Assets::clear();
}

void Game::close() {
	renderer.close();
	window.close();
	SDL_Quit();
}

