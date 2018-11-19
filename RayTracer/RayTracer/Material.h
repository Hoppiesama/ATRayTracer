#pragma once
#include "Vector3.h"

class Material
{
public:
	Material();
	~Material();

	Vector3 diffuseColour;

	void SetDiffuseColour(Vector3 _colour) { diffuseColour = _colour; }
	Vector3 GetDiffuseColour() { return diffuseColour; }

	//TODO - needs an index of refraction (IoR)
	double indexOfRefraction = 1; // Default is 1 for no refraction
};

