#pragma once

#include "Component.h"
#include "Vector2.h"

//CCC = CircleCollisionComponent

class CCC : public Component {

public:
	CCC(Actor* owner);
	CCC() = delete;
	CCC(const CCC&) = delete;

	CCC& operator = (const CCC&) = delete;

	float getRadius() const;
	void setRadius(float radiusP);

	const Vector2 getCenter() const;

private:
	float radius;
};

bool Intersect(const CCC& a, const CCC& b);
