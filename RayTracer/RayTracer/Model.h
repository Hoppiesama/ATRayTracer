#pragma once
#include "Object.h"
#include <vector>
#include "VertexStructs.h"
#include "BoundingVolumeHierarchy.h"

class Triangle : public Object
{
public:
	Triangle(Vertex3 _A, Vertex3 _B, Vertex3 _C)
		: vertA(_A), vertB(_B), vertC(_C)
	{
	}
	~Triangle() = default;

	Vertex3 vertA;
	Vertex3 vertB;
	Vertex3 vertC;

	void offsetPositions()
	{
		vertA.pos = vertA.pos + position;
		vertB.pos = vertB.pos + position;
		vertC.pos = vertC.pos + position;
	}

	virtual double GetLowestXVert() override
	{ 
		if(vertA.pos.x < vertB.pos.x && vertA.pos.x < vertC.pos.x)
		{
			return vertA.pos.x;
		}
		else if(vertB.pos.x < vertC.pos.x)
		{
			return vertB.pos.x;
		}
		else
		{
			return vertC.pos.x;
		}
	};
	virtual double GetHighestXVert() override 
	{
		if(vertA.pos.x > vertB.pos.x && vertA.pos.x > vertC.pos.x)
		{
			return vertA.pos.x;
		}
		else if(vertB.pos.x > vertC.pos.x)
		{
			return vertB.pos.x;
		}
		else
		{
			return vertC.pos.x;
		}
	};
	virtual double GetLowestYVert() override
	{
		if(vertA.pos.y < vertB.pos.y && vertA.pos.y < vertC.pos.y)
		{
			return vertA.pos.y;
		}
		else if(vertB.pos.y < vertC.pos.y)
		{
			return vertB.pos.y;
		}
		else
		{
			return vertC.pos.y;
		}
	};
	virtual double GetHighestYVert() override
	{
		if(vertA.pos.y > vertB.pos.y && vertA.pos.y > vertC.pos.y)
		{
			return vertA.pos.y;
		}
		else if(vertB.pos.y > vertC.pos.y)
		{
			return vertB.pos.y;
		}
		else
		{
			return vertC.pos.y;
		}
	};
	virtual double GetLowestZVert() override
	{
		if(vertA.pos.z < vertB.pos.z && vertA.pos.z < vertC.pos.z)
		{
			return vertA.pos.z;
		}
		else if(vertB.pos.z < vertC.pos.z)
		{
			return vertB.pos.z;
		}
		else
		{
			return vertC.pos.z;
		}
	};
	virtual double GetHighestZVert() override
	{
		if(vertA.pos.z > vertB.pos.z && vertA.pos.z > vertC.pos.z)
		{
			return vertA.pos.z;
		}
		else if(vertB.pos.z > vertC.pos.z)
		{
			return vertB.pos.z;
		}
		else
		{
			return vertC.pos.z;
		}
	};

	//Source: scratchapixel.
	double intersect(const Ray& r) const override
	{
		Vector3 v0v1 = vertB.pos - vertA.pos;
		Vector3 v0v2 = vertC.pos - vertA.pos;
		Vector3 pvec = r.GetDirection().cross(v0v2);
		float determinant = v0v1.dot(pvec); 

		// if the determinant is negative the triangle is backfacing
		// if the determinant is close to 0, the ray misses the triangle
		// ray and triangle are parallel if det is close to 0


		if (fabs(determinant) < DBL_EPSILON) return 0.0; 
		double invDet = 1 / determinant; 
 
		Vector3 tvec = r.GetOrigin() - vertA.pos;
		double u = tvec.dot(pvec) * invDet; 
		if (u < 0.0 || u > 1.0) 
			return 0.0; 
 
		Vector3 qvec = tvec.cross(v0v1);
		double v = r.GetDirection().dot(qvec) * invDet;
		if (v < 0 || u + v > 1) return 0.0; 
 
		double t = v0v2.dot(qvec) * invDet;
 
		if(t > DBL_EPSILON) // ray intersection
		{
			return t;
		}
		else // This means that there is a line intersection but not a ray intersection.
		{
			return 0.0;
		}

	}

};

class Model : public Object
{

public:
	Model(Vector3 _position);

	Model(std::vector<Vertex3> _verts, std::vector<int> _indices, std::vector<int> _faceIndices, Material _mat)
		: indices(_indices), vertices(_verts), faceIndex(_faceIndices)
	{
		int incrementor = 0;

		material = _mat;

		for(int a = 0; a < faceIndex.size(); a++)
		{
			triangles.push_back(Triangle(vertices[indices[incrementor] - 1], vertices[indices[incrementor + 1] - 1], vertices[indices[incrementor + 2] - 1]) ) ;
			triangles.back().material = material;
			triangles.back().position = { 0.0,0.0,0.0 };
			incrementor += faceIndex[a];
		}
	};

	~Model();

	void InitTriangles();
	
	std::vector<Triangle>& getTriangles() { return triangles; }

	std::vector<int> faceIndex;
	std::vector<Vertex3> vertices;
	std::vector<int> indices;

	std::vector<Triangle> triangles;

	// Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
	double intersect(const Ray& r) const override;

	BoundingVolumeHierarchy localBVH;

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

/*

Face Index Array	
	Number of vertices each face is made of	{3, 3}
Vertex Index Array	
	Indices of the vertices making up each face in the vertex array	{0, 2, 1, 0, 3, 2}
Vertex Array	
	Vertex position	{{-1,0,0}, {0,1,0}, {1,0,0}, {0,-1,0}}

*/