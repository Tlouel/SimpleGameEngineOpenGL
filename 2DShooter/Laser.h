#pragma once

#include "Actor.h"
#include "CCC.h"

class Laser : public Actor {

public:
	Laser();

	void updateActor(float dt) override;

private:
	CCC* collision;
	float deathTimer;
};
