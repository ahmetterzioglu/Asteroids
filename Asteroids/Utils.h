#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
const float SCREEN_WIDTH = 800;
const float SCREEN_HEIGHT = 600;
const float DEG_TO_RAD = 0.0174532925;
const float DRAG_MULTIPLIER = 0.0075;
const float TURNSPEED = 100;
const float MAXSPEED = 4;
const Vector2f PLAYER_SIZE = Vector2f(25, 50);
const Vector2f THRUSTER_SIZE = Vector2f(12, 8);
const float THRUSTER_OFFSET = 25;
const Vector2f START_POSITION = Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT /2);
const float ACCELERATION = 0.04;
Vector2f normalizeVector(Vector2f vector);
float distanceBetween(Vector2f v1, Vector2f v2);
float degreesToReadians(float degrees);
Vector2f getDirectionVectorFromDegrees(float degree);
float vectorMagnitude(Vector2f vector);
Vector2f oppositeVector(Vector2f vector);