#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "Maths.h"


#include <algorithm>

Actor::Actor() : state(Actor::ActorState::Active), position(Vector3::zero), scale(1.0f), rotation(Quaternion::identity), 
	mustRecomputeWorldTransform(true), game(Game::instance()) {
	
	game.addActor(this);
}

Actor::~Actor() {
	game.removeActor(this);
	
	while (!components.empty()) {
		delete components.back();
	}
}

void Actor::setPosition(Vector3 positionP) {
	position = positionP;
	mustRecomputeWorldTransform = true;
}

void Actor::setScale(float scaleP) {
	scale = scaleP;
	mustRecomputeWorldTransform = true;
}

void Actor::setRotation(Quaternion rotationP) {
	rotation = rotationP;
	mustRecomputeWorldTransform = true;
}

void Actor::setState(ActorState stateP) {
	state = stateP;
}

Vector3 Actor::getForward() const {
	return Vector3::transform(Vector3::unitX, rotation);
}

void Actor::computeWorldTransform() {
	if (mustRecomputeWorldTransform) {
		mustRecomputeWorldTransform = false;

		worldTransform = Matrix4::createScale(scale);
		worldTransform *= Matrix4::createFromQuaternion(rotation);
		worldTransform *= Matrix4::createTranslation(position);

		for (auto component : components) {
			component->onUpdateWorldTransform();
		}
	}
	
}

void Actor::processInput(const Uint8* keyState) {
	
	if (state == Actor::ActorState::Active) {
		for (auto component : components) {
			component->processInput(keyState);
		}

		actorInput(keyState);
	}
}

void Actor::actorInput(const Uint8* keyState) {}

void Actor::updateActor(float dt) {}

void Actor::update(float dt) {
	
	if (state == Actor::ActorState::Active) {
		 
		computeWorldTransform();
		updateComponents(dt);
		updateActor(dt);
		computeWorldTransform();
		
	}
}

void Actor::updateComponents(float dt) {
	for (auto component : components)
	{
		component->update(dt);
	}
}

void Actor::addComponent(Component* component) {

	int myOrder = component->getUpdateOrder();
	auto iter = begin(components);
	for (; iter != end(components); ++iter) { // auto = compilateur detecte le type(car trop long) / for (; iter != end(components); ++iter) : parcourt tous les component quand il yen a un sup?rieur a notre iteror s'arr?te, ; -> car on a besoin de trois ?l?ments MAIS on a d?j? un d?fini avant auto iter

		if (myOrder < (*iter)->getUpdateOrder()) {

			break;
		}
	}

	components.insert(iter, component);
}



void Actor::removeComponent(Component* component) {

	auto iter = std::find(begin(components), end(components), component);
	if (iter != end(components)) {

		components.erase(iter);
	}
}