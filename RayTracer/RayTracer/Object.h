#pragma once
#include "Material.h"

class Object
{

public:
	Object() = default;
	~Object() = default;

	Vector3 position;

	Material material;

private:
};

