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

Vector2i getBucket(Vector2f pos)
{
	int col = int(pos.x / BUCKET_SIZE);
	if (col < 0)
		col = 0;
	else if (col >= SCREEN_WIDTH/BUCKET_SIZE )
		col = SCREEN_WIDTH / BUCKET_SIZE - 1;

	int row = int(pos.y / BUCKET_SIZE);
	if (row < 0)
		row = 0;
	else if (row >= SCREEN_HEIGHT / BUCKET_SIZE)
		row = SCREEN_HEIGHT / BUCKET_SIZE - 1;

	return Vector2i(col, row);
}

Vector2f wrapAround(Vector2f position) {
	if (position.x > SCREEN_WIDTH) {
		position.x = 0;
	}
	if (position.x < 0) {
		position.x = SCREEN_WIDTH;
	}
	if (position.y > SCREEN_HEIGHT) {
		position.y = 0;
	}
	if (position.y < 0) {
		position.y = SCREEN_HEIGHT;
	}
	return position;
}
