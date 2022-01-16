#include "CCC.h"
#include "Actor.h"

//CCC = CircleCollisionComponent

CCC::CCC(Actor* owner) : Component(owner), radius(1.0f) {}

float CCC::getRadius() const {
	return owner.getScale() * radius;
}

void CCC::setRadius(float radiusP) {
	radius = radiusP;
}

const Vector2 CCC::getCenter() const {
	Vector3 position = owner.getPosition();
	return Vector2(position.y, position.z);
}

bool Intersect(const CCC& a, const CCC& b) {
	Vector2 aCenter = a.getCenter();
	Vector2 bCenter = b.getCenter();
	Vector2 ab = bCenter - aCenter;

	float distSq = ab.lengthSq();
	float sumOfRadius = a.getRadius() + b.getRadius();

	return distSq <= sumOfRadius * sumOfRadius;
}