#pragma once
#include "Vector3.h"

class Transform
{
public:
	Transform() = default;
	Transform(Vector3 _pos, Vector3 _rot, Vector3 _scale) : position(_pos), rotation(_rot), scale(_scale) {};
	~Transform() = default;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;


};

