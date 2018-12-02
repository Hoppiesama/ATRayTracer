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

	double intersect(const Ray& r) const override
	{
		// compute plane's normal

		//V0 is always the root vertes, must ensure v1 and v2 are in an anticlockwise ordering.
		Vector3 v0v1 = vertB.pos - vertA.pos;
		Vector3 v0v2 = vertC.pos - vertA.pos;
		// no need to normalize
		Vector3 triangleNormal = v0v1.cross(v0v2); // N 
		double area2 = triangleNormal.magnitude();

		// Step 1: finding P
		// check if ray and plane are parallel ?
		double NdotRayDirection = triangleNormal.dot(r.GetDirection());
		if(fabs(NdotRayDirection) < 0.001) // almost 0 
			return false; // they are parallel so they don't intersect ! 

						  // compute d parameter using equation 2
		double d = triangleNormal.dot(vertA.pos);

		// compute t (equation 3)
		double t = (triangleNormal.dot(r.GetOrigin()) + d) / NdotRayDirection;

		// check if the triangle is in behind the ray
		if(t < 0) return false; // the triangle is behind 

								// compute the intersection point using equation 1
		Vector3 hitPoint = r.GetOrigin() + r.GetDirection() * t;

		// Step 2: inside-outside test
		Vector3 insideOutCheck; // vector perpendicular to triangle's plane 

								// edge 0
		Vector3 edge0 = vertB.pos - vertA.pos;
		Vector3 vp0 = hitPoint - vertA.pos;
		insideOutCheck = edge0.cross(vp0);
		if(triangleNormal.dot(insideOutCheck) < 0) return false; // P is on the right side 

																 // edge 1
		Vector3 edge1 = vertC.pos - vertB.pos;
		Vector3 vp1 = hitPoint - vertB.pos;
		insideOutCheck = edge1.cross(vp1);
		if(triangleNormal.dot(insideOutCheck) < 0)  return false; // P is on the right side 

																  // edge 2
		Vector3 edge2 = vertA.pos - vertC.pos;
		Vector3 vp2 = hitPoint - vertC.pos;
		insideOutCheck = edge2.cross(vp2);
		if(triangleNormal.dot(insideOutCheck) < 0) return false; // P is on the right side; 

		return true; // this ray hits the triangle 
	}
};

class Model : public Object
{

public:
	Model(Vector3 _position);

	Model(std::vector<Vertex3> _verts, std::vector<int> _indices, std::vector<int> _faceIndices)
		: indices(_indices), vertices(_verts), faceIndex(_faceIndices)
	{
		int incrementor = 0;

		for(int a = 0; a < faceIndex.size(); a++)
		{
			triangles.push_back(std::move(Triangle(vertices[indices[incrementor]], vertices[indices[incrementor + 1]], vertices[indices[incrementor + 2]]) ) );
			triangles.back().colour = this->colour;
			triangles.back().emission = this->emission;
			triangles.back().material = this->material;
			triangles.back().refl = this->refl;
			triangles.back().position = { 0.0,0.0,0.0 };
			incrementor += faceIndex[a];
		}
	};

	~Model();

	void InitTriangles();
	
	std::vector<Triangle> getTriangles() { return triangles; }

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