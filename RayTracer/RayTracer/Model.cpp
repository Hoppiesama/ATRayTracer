#include "Model.h"

Model::Model(Vector3 _position)
{
	position = _position;
}

Model::~Model()
{
}

void Model::InitTriangles()
{
	int size = (int)indices.size() / 3;

	for(int x = 0; x < size; x++)
	{
		faceIndex.push_back(3);
	}

	int incrementor = 0;

	for(int a = 0; a < faceIndex.size(); a++)
	{
		//triangles.push_back(&Triangle(vertices[indices[incrementor]], vertices[indices[incrementor + 1]], vertices[indices[incrementor + 2]]));
		triangles.push_back(std::move(Triangle(vertices[indices[incrementor]], vertices[indices[incrementor + 1]], vertices[indices[incrementor + 2]])));
		triangles.back().colour = colour;
		triangles.back().emission = emission;
		triangles.back().material = material;
		triangles.back().refl = refl;
		triangles.back().position = { 0.0,0.0,0.0 };
		incrementor += faceIndex[a];
	}
}

// Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution

double Model::intersect(const Ray& r) const
{
	// compute plane's normal

	//V0 is always the root vertes, must ensure v1 and v2 are in an anticlockwise ordering.
	Vector3 v0v1 = v1 - v0;
	Vector3 v0v2 = v2 - v0;
	// no need to normalize
	Vector3 triangleNormal = v0v1.cross(v0v2); // N 
	double area2 = triangleNormal.magnitude();

	// Step 1: finding P
	// check if ray and plane are parallel ?
	double NdotRayDirection = triangleNormal.dot(r.GetDirection());
	if(fabs(NdotRayDirection) < 0.001) // almost 0 
		return false; // they are parallel so they don't intersect ! 

					  // compute d parameter using equation 2
	double d = triangleNormal.dot(v0);

	// compute t (equation 3)
	double t = (triangleNormal.dot(r.GetOrigin()) + d) / NdotRayDirection;

	// check if the triangle is in behind the ray
	if(t < 0) return false; // the triangle is behind 

							// compute the intersection point using equation 1
	Vector3 hitPoint = r.GetOrigin() + r.GetDirection() * t;

	// Step 2: inside-outside test
	Vector3 insideOutCheck; // vector perpendicular to triangle's plane 

							// edge 0
	Vector3 edge0 = v1 - v0;
	Vector3 vp0 = hitPoint - v0;
	insideOutCheck = edge0.cross(vp0);
	if(triangleNormal.dot(insideOutCheck) < 0) return false; // P is on the right side 

															 // edge 1
	Vector3 edge1 = v2 - v1;
	Vector3 vp1 = hitPoint - v1;
	insideOutCheck = edge1.cross(vp1);
	if(triangleNormal.dot(insideOutCheck) < 0)  return false; // P is on the right side 

															  // edge 2
	Vector3 edge2 = v0 - v2;
	Vector3 vp2 = hitPoint - v2;
	insideOutCheck = edge2.cross(vp2);
	if(triangleNormal.dot(insideOutCheck) < 0) return false; // P is on the right side; 

	return true; // this ray hits the triangle 
}


//TODO - foreach of these, calculate the vertex position (after transformation using the World matrix)
double Model::GetLowestXVert()
{
	return 0.0;
}

double Model::GetHighestXVert()
{
	return 0.0;
}

double Model::GetLowestYVert()
{
	return 0.0;
}

double Model::GetHighestYVert()
{
	return 0.0;
}

double Model::GetLowestZVert()
{
	return 0.0;
}

double Model::GetHighestZVert()
{
	return 0.0;
}
