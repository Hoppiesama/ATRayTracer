#pragma once
#include "Vector3.h"


class Camera
{
public:
	~Camera() = default;
	Camera(Vector3 _position, Vector3 _lookAt, int widthRes, int heightRes, double _fov)
	{
		position = _position;
		lookAtTargetPos = _lookAt;
		lookDirection = (lookAtTargetPos - position).normalize();

		Vector3 worldUp(0., 1., 0.);
		Vector3 camRight = lookDirection % worldUp; //% == cross product override
		camRightFOVAdjusted = camRight * tan(_fov / 2);
		camUpFOVAdjusted = (camRightFOVAdjusted % lookDirection) * heightRes / widthRes;
	}


	Vector3 position;
	Vector3 lookAtTargetPos;
	Vector3 lookDirection;

	Vector3 GetRightFOVAdjusted() { return camRightFOVAdjusted; }
	Vector3 GetUpFOVAdjusted() { return camUpFOVAdjusted; }

private:
	Vector3 camRightFOVAdjusted;
	Vector3 camUpFOVAdjusted;

};