#pragma once
#include "Ray.h"
#include "Object.h"


class Sphere : public Object
{
public:
	Sphere() = default;
	~Sphere() = default;

	//Sphere(Vector3 centre, double _radius) { position = centre; radius = _radius; }
	//virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

	double radius;       // radius 

	Sphere(double rad_, Vector3 p_, Vector3 e_, Vector3 c_, SurfaceType refl_, double _indexOfRefraction) 
	{
		radius = rad_;
		material = new Material(c_, e_, refl_);
		position = p_;
		material->SetIndexOfRefraction(_indexOfRefraction);
	}

	double intersect(const Ray &r, double& _uvU, double& _uvV) const override;


	double GetLowestXVert() { return position.x - radius; }
	double GetHighestXVert() { return position.x + radius; }

	double GetLowestYVert() { return position.y - radius; }
	double GetHighestYVert() { return position.y + radius; }

	double GetLowestZVert() { return position.z - radius; }
	double GetHighestZVert() { return position.z + radius; }

};
