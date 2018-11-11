#pragma once
#include "Object.h"
#include "Ray.h"

class Camera : public Object
{
public:
	Camera();
	~Camera();
	Camera(Vector3 _position, Vector3 _lookAt) { transform.position = _position; lookAt = _lookAt; }

	Vector3 GetLookAt() const { return lookAt; }

private:
	Vector3 lookAt;
};

