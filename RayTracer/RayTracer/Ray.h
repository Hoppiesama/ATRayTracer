#pragma once
#include "Vector3.h"

class Ray
{
public:
	Ray() = default;
	Ray(Vector3 _origin, Vector3 _direction) : origin(_origin), direction(_direction){	};
	~Ray() = default;

	//p(t) = A + t*B
	Vector3 pointAtParameter(float t) const { return origin + (direction * t); }

	Vector3 GetOrigin() const { return origin; }
	Vector3 GetDirection() const { return direction; }

private:
	Vector3 origin;
	Vector3 direction;
};