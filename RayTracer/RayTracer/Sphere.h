#pragma once
#include "Object.h"
#include "Ray.h"

class Sphere : public Object
{
public:
	Sphere() = default;
	~Sphere() = default;

	Sphere(Vector3 centre, float _radius) { transform.position = centre; radius = _radius; }
	//virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	bool isHit(Ray _ray, float& t);

	float radius;
};


