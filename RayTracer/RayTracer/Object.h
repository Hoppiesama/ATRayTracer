#pragma once
#include "Material.h"
#include "Ray.h"

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

	virtual double intersect(const Ray &r) const = 0;

	virtual double GetLowestXVert() = 0;
	virtual double GetHighestXVert() = 0;

	virtual double GetLowestYVert() = 0;
	virtual double GetHighestYVert() = 0;

	virtual double GetLowestZVert() = 0;
	virtual double GetHighestZVert() = 0;

private:

};

