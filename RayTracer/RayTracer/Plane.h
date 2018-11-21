#pragma once
#include "Object.h"

class Plane : public Object
{
public:
	Plane();
	~Plane();

	//TODO implement plane intersection
	double intersect(const Ray &r) const override { return 1.0; }
};

