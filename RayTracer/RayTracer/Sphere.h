#pragma once
#include "Ray.h"
#include "Object.h"

enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance() 


class Sphere : public Object
{
public:
	Sphere() = default;
	~Sphere() = default;

	//Sphere(Vector3 centre, double _radius) { position = centre; radius = _radius; }
	//virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	double radius;       // radius 
	Vector3  emission, colour;

	Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive) 

	Sphere(double rad_, Vector3 p_, Vector3 e_, Vector3 c_, Refl_t refl_) :
		radius(rad_), emission(e_), colour(c_), refl(refl_)
	{
		position = p_;
	}

	double intersect(const Ray &r) const;


	double GetLowestXVert() { return position.x - radius; }
	double GetHighestXVert() { return position.x + radius; }

	double GetLowestYVert() { return position.y - radius; }
	double GetHighestYVert() { return position.y + radius; }

	double GetLowestZVert() { return position.y - radius; }
	double GetHighestZVert() { return position.y + radius; }

};
