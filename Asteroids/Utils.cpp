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

Vector2f wrapAround(Vector2f position, float x, float y) {

	if (position.x - x > SCREEN_WIDTH) {
		position.x = 0 -x;
	}
	if (position.x + x < 0) {
		position.x = SCREEN_WIDTH + x;
	}
	if (position.y - y > SCREEN_HEIGHT) {
		position.y = 0 - x;
	}
	if (position.y + y < 0) {
		position.y = SCREEN_HEIGHT + y;
	}
	return position;
}

bool doCirclesCollide(Vector2f p1, float r1, Vector2f p2, float r2) {
	Vector2f o1 = p1 + Vector2f(r1, r1);
	Vector2f o2 = p2 + Vector2f(r2, r2);
	float distance = distanceBetween(p1, p2);
	return distance < (r1 + r2);
}

bool doesCircleAndPointCollide(Vector2f position, float r1, Vector2f point) {
	float distance = distanceBetween(position , point);
	return distance < r1;
}

bool isPointInRectangle(Vector2f rectanglePos, Vector2f renctangleSize, Vector2i point) {
	return (point.x >= rectanglePos.x) && (point.x <= rectanglePos.x + renctangleSize.x)
		&& (point.y >= rectanglePos.y) && (point.y <= rectanglePos.y + renctangleSize.y);
}

