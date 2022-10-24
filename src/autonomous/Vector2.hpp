#pragma once

#include "../Constants.hpp"
using namespace Constants;

struct Vector2{
    double x;
    double y;

	Vector2(double x = 0.0, double y = 0.0):x(x),y(y){}
	
	Vector2 operator+(Vector2 otherVector){
		return {x + otherVector.x, y + otherVector.y};
	}

	Vector2 operator-(Vector2 otherVector){
		return {x - otherVector.x, y - otherVector.y};
	}

	Vector2 operator*(Vector2 otherVector){
		return {x * otherVector.x, y * otherVector.y};
	}

	Vector2 operator/(Vector2 otherVector){
		return {x / otherVector.x, y / otherVector.y};
	}

	inline float distanceTo(Vector2 otherVector) {
		float dx = otherVector.x-x;
		float dy = otherVector.y-y;
		return f_sqrt((dx*dx)+(dy*dy));
	}

	inline float degreesTo(float currentHeading, Vector2 pointToFace) {
		float dx = pointToFace.x - x;
		float dy = pointToFace.y - y;
		return f_atan(dy/dx);
	}
};