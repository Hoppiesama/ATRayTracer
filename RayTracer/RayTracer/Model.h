#pragma once
#include "Object.h"
#include <vector>
#include "VertexStructs.h"

class Model : public Object
{
public:
	Model();
	~Model();

	std::vector<Vertex3> vertices;
	std::vector<int> indices;

	// Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
	double intersect(const Ray& r) const override;

private:

	Vector3 v0;
	Vector3 v1;
	Vector3 v2;

	// Inherited via Object
	virtual double GetLowestXVert() override;
	virtual double GetHighestXVert() override;
	virtual double GetLowestYVert() override;
	virtual double GetHighestYVert() override;
	virtual double GetLowestZVert() override;
	virtual double GetHighestZVert() override;
};