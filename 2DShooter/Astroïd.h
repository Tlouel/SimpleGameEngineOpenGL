#pragma once

#include "Actor.h"
#include "CCC.h"

class Astroid : public Actor {

public:
	Astroid();
	~Astroid();

	CCC& getCollision() { return *collision; }

private:
	CCC* collision;
};