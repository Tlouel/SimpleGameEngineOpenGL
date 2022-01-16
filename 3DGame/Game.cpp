#include "Game.h"
#include "Timer.h"
#include "Assets.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "AnimSpriteComponent.h"
#include "BackgroundSpriteComponent.h"
#include "MeshComponent.h"
#include "Camera.h"


#include <vector>

using std::vector;

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);


	return isWindowInit && isRendererInit; // Return bool && bool && bool ...to detect error
}

void Game::load() {

	//Load Textures
	Assets::loadTexture(renderer, "Resources\\Textures\\HealthBar.png", "HealthBar");
	Assets::loadTexture(renderer, "Resources\\Textures\\Plane.png", "Plane");
	Assets::loadTexture(renderer, "Resources\\Textures\\Radar.png", "Radar");
	Assets::loadTexture(renderer, "Resources\\Textures\\Sphere.png", "Sphere");
	Assets::loadTexture(renderer, "Resources\\Textures\\Default.png", "Default");
	Assets::loadTexture(renderer, "Resources\\Textures\\Cube.png", "Cube");

	//Load Shaders
	Assets::loadShader("Shaders\\Basic.vert", "Shaders\\Basic.frag", "", "", "", "Basic");
	Assets::loadShader("Shaders\\Transform.vert", "Shaders\\Basic.frag", "", "", "", "Transform");
	Assets::loadShader("Shaders\\Sprite.vert", "Shaders\\Sprite.frag", "", "", "", "Sprite");
	Assets::loadShader("Shaders\\BasicMesh.vert", "Shaders\\BasicMesh.frag", "", "", "", "BasicMesh");

	//Load Meshes
	Assets::loadMesh("Meshes\\Cube.gpmesh", "Mesh_Cube");
	Assets::loadMesh("Meshes\\Plane.gpmesh", "Mesh_Plane");
	Assets::loadMesh("Meshes\\Sphere.gpmesh", "Mesh_Sphere");


	Actor* ui = new Actor();
	ui->setPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(ui, Assets::getTexture("HealthBar"));

	camera = new Camera();

	Actor* a = new Actor();
	a->setPosition(Vector3(200.0f, 105.0f, 0.0f));
	a->setScale(100.0f);

	Quaternion q(Vector3::unitY, -Maths::piOver2);
	q = Quaternion::concatenate(q, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi / 4.0f));
	a->setRotation(q);

	MeshComponent* mc = new MeshComponent(a);
	mc->setMesh(Assets::getMesh("Mesh_Cube"));

	Actor* b = new Actor();
	b->setPosition(Vector3(200.0f, -75.0f, 0.0f));
	b->setScale(3.0f);

	MeshComponent* mcb = new MeshComponent(b);
	mcb->setMesh(Assets::getMesh("Mesh_Sphere"));
	

	// Controlled ship
	/*Ship* ship = new Ship();
	ship->setPosition(Vector2{ 100, 300 });*/


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
		pendingActor->computeWorldTransform();
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

