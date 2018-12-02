#pragma once
#include "Material.h"
#include "Ray.h"

enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance() 

enum ObjType
{
	Sph,
	Mod,
};

class Object
{

public:
	Object() = default;
	virtual ~Object() = default;

	Vector3 position;

	Material material;

	ObjType type = Sph;

	//TODO - replace with material
	Vector3  emission, colour;
	Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive) 

	virtual double intersect(const Ray &r) const = 0;

	virtual double GetLowestXVert() = 0;
	virtual double GetHighestXVert() = 0;

	virtual double GetLowestYVert() = 0;
	virtual double GetHighestYVert() = 0;

	virtual double GetLowestZVert() = 0;
	virtual double GetHighestZVert() = 0;

private:

};

