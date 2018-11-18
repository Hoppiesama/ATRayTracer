#pragma once
#include "Vector3.h"


class Camera
{
public:
	Camera() = default;
	~Camera() = default;
	Camera(Vector3 _position, Vector3 _lookAt)
	{
		position = _position;
		lookAtTargetPos = _lookAt;
		lookDirection = (lookAtTargetPos - position).normalize();

		camRight = lookDirection % up; //% == cross product override
	}

	Vector3 position;
	Vector3 up = { 0,1,0 };
	Vector3 camRight;

	Vector3 lookAtTargetPos;
	Vector3 lookDirection;
};