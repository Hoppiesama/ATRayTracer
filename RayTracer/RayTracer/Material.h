#pragma once
#include "Vector3.h"

enum SurfaceType { DIFFUSE, SPEC_REFLECTION, REFRACTION };  // material types, used in radiance() 

class Material
{
public:
	Material(Vector3 _diffColour, Vector3 _emission, SurfaceType _type)
		: diffuseColour(_diffColour), emission(_emission), surface(_type){};
	Material() = default;
	~Material() = default;

	void SetDiffuseColour(Vector3 _colour) { diffuseColour = _colour; }
	Vector3 GetDiffuseColour() const { return diffuseColour; }

	void SetEmission(Vector3 _colour) { diffuseColour = _colour; }
	Vector3 GetEmission() const { return diffuseColour; }

	SurfaceType surface;

	double GetIndexOfRefraction() const { return indexOfRefraction; }
	void SetIndexOfRefraction(double _val) { indexOfRefraction = _val; }

private:
	Vector3 diffuseColour;
	Vector3 emission;

	// Default is 1 for no refraction
	double indexOfRefraction = 1;
};

