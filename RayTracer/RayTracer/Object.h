#pragma once
#include "Material.h"
#include "Ray.h"

class Object
{

public:
	Object() = default;
	virtual ~Object() = default;

	Vector3 position;

	Material material;

	virtual double intersect(const Ray &r) const = 0;

private:
};

