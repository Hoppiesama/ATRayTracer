#include "Sphere.h"
#include <cmath>

bool Sphere::isHit(Ray _ray, float & t)
{
	Vector3 origin = _ray.GetOrigin();
	Vector3 direction = _ray.GetDirection();

	Vector3 rayToCentre = origin - transform.position;

	float b = 2 * Vector3::dot(rayToCentre, direction);
	float c = Vector3::dot(rayToCentre, rayToCentre) - radius * radius;

	float disc = b * b - 4 * c;

	if(disc < 0)
	{
		return false;
	}
	else
	{
		disc = sqrt(disc);
		float t0 = -b - disc;
		float t1 = -b + disc;

		t = (t0 < t1) ? t0 : t1;

		return true;
	}
}
