#pragma once

#include "Vector2.h"
#include "Matrix4.h"

#include <vector>
#include <SDL_stdinc.h>

using std::vector;

class Game;
class Component;

class Actor {

public:
	enum class ActorState {
		Active,
		Paused,
		Dead
	};

	Actor();
	virtual ~Actor();

	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;


	Game& getGame() const { return game; }

	const ActorState getState() const { return state; }
	const Vector2 getPosition() const { return position; }

	const float getScale() const { return scale; }
	const float getRotation() const { return rotation; }

	const Matrix4& getWorldTransform() const { return worldTransform; }

	void computeWorldTransform();


	void setPosition(Vector2 positionP);
	void setScale(float scaleP);
	void setRotation(float rotationP);
	void setState(ActorState stateP);

	void update(float dt);
	void updateComponents(float dt);
	virtual void updateActor(float dt);

	void addComponent(Component* component);
	void removeComponent(Component* component);

	Vector2 getForward() const;

	void processInput(const Uint8* keyState);
	virtual void actorInput(const Uint8* keyState);

private:

	Game& game;
	ActorState state;

	Vector2 position;
	float scale;
	float rotation;

	vector<Component*> components;

	Matrix4 worldTransform;
	bool mustRecomputeWorldTransform;
};