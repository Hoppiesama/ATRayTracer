#pragma once
#include "Ray.h"

enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance() 


class Sphere
{
public:
	Sphere() = default;
	~Sphere() = default;

	Sphere(Vector3 centre, double _radius) { position = centre; rad = _radius; }
	//virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	double rad;       // radius 
	Vector3 position, emission, colour;

	Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive) 

	Sphere(double rad_, Vector3 p_, Vector3 e_, Vector3 c_, Refl_t refl_) :
		rad(rad_), position(p_), emission(e_), colour(c_), refl(refl_)
	{

	}

	double intersect(const Ray &r) const;

};
