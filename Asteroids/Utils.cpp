#include "Utils.h"

Vector2f normalizeVector(Vector2f vector) {
	float x = vector.x;
	float y = vector.y;
	float length = sqrtf(x*x + y*y);
	x = x / length;
	y = y / length;
	return Vector2f(x, y);
}

float distanceBetween(Vector2f v1, Vector2f v2) {
	Vector2f diff = v2 - v1;
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}

float degreesToReadians(float degrees) {
	return (degrees - 90) * DEG_TO_RAD;
}

Vector2f getDirectionVectorFromDegrees(float degree) {
	return normalizeVector(Vector2f(cos(degreesToReadians(degree)), sin(degreesToReadians(degree))));
}

float vectorMagnitude(Vector2f vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

Vector2f oppositeVector(Vector2f vector) {
	return Vector2f(-vector.x, -vector.y);
}