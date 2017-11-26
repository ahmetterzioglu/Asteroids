#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
using namespace sf;
const float SCREEN_WIDTH = 1024;
const float SCREEN_HEIGHT = 768;
const float DEG_TO_RAD = 0.0174532925;
const float DRAG_MULTIPLIER = 0.0075;
const float TURNSPEED = 100;
const float MAXSPEED = 1;
const int PLAYER_LIVES = 3;
const int ASTEROID_LIVES = 3;
const float BUCKET_SIZE = 128;
const int BUCKET_COLS = 8;
const int BUCKET_ROWS = 6;
const int GOPLAYER = 0;
const int GOLASER = 1;
const int GOASTEROID = 2;
const float LASER_SPEED = 500;
const float LASER_OFFSET = 20;
const float LASER_COOLDOWN = 0.5;
const float LASER_TIMER = 2;
const Vector2f LASER_SIZE = Vector2f(2, 10);
const float PLAYER_SIZE =25;
const Vector2f THRUSTER_SIZE = Vector2f(20, 20);
const float THRUSTER_OFFSET = 25;
const Vector2f START_POSITION = Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT /2);
const float ACCELERATION = 0.04;
const float ASTEROID_SIZE = 25;
const float ASTEROID_SPEED = 20;
Vector2f normalizeVector(Vector2f vector);
float distanceBetween(Vector2f v1, Vector2f v2);
float degreesToReadians(float degrees);
Vector2f getDirectionVectorFromDegrees(float degree);
float vectorMagnitude(Vector2f vector);
Vector2i getBucket(Vector2f pos);
Vector2f wrapAround(Vector2f position);
bool doCirclesCollide(Vector2f p1, float r1, Vector2f p2, float r2);
bool doesCircleAndPointCollide(Vector2f origin, float r1, Vector2f point);
