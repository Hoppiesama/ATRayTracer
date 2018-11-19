#pragma once
#include "Sphere.h"
#include "Vector3.h"
#include <vector>
#include <memory>
#include "Ray.h"

//https://coocoscode.wordpress.com/2010/12/15/building-bounding-volume-hiearchies/
//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

enum LeftOrRight
{
	LEFT,
	RIGHT,
};

enum Axis
{
	X,
	Y,
	Z
};


class BoundingBox
{
public:
	BoundingBox() = default;
	BoundingBox(Vector3 _bottomBackLeftCorner, Vector3 _topFrontRightCorner)
	{
		minMaxCorners = std::make_pair(_bottomBackLeftCorner, _topFrontRightCorner);
	}

	void SetBounds(std::pair<Vector3, Vector3> _bounds)
	{
		minMaxCorners = _bounds;
	}

	std::pair<Vector3, Vector3> GetBounds()
	{
		return minMaxCorners;
	}

	Vector3 GetExtents()
	{
		return Vector3(abs(minMaxCorners.first.x - minMaxCorners.second.x), abs(minMaxCorners.first.y - minMaxCorners.second.y), abs(minMaxCorners.first.z - minMaxCorners.second.z));
	}

	bool intersect(Ray& ray)
	{
		return false;

		Vector3 rayDirection = ray.GetDirection();

		//Get the minimum and maximum that t can be based on this intersectioning concept.
		//https://www.scratchapixel.com/images/upload/ray-simple-shapes/2dfig.png


		//Do it for x
		double txMin = (minMaxCorners.first.x - ray.GetOrigin().x) / rayDirection.x;
		double txMax = (minMaxCorners.second.x - ray.GetOrigin().x) / rayDirection.x;

		double temp;

		if(txMax < txMin)
		{
			temp = txMax; 
			txMax = txMin; 
			txMin = temp;
		}

		double tyMin = (minMaxCorners.first.y - ray.GetOrigin().y) / rayDirection.y;
		double tyMax = (minMaxCorners.second.y - ray.GetOrigin().y) / rayDirection.y;


		//then do it for y
		if(tyMax < tyMin)
		{
			temp = tyMax;
			tyMax = tyMin;
			tyMin = temp;
		}


		//then do it for z
		double tzMin = (minMaxCorners.first.z - ray.GetOrigin().z) / rayDirection.z;
		double tzMax = (minMaxCorners.second.z - ray.GetOrigin().z) / rayDirection.z;


		if(tzMax < tzMin)
		{
			temp = txMax;
			tzMax = tzMin;
			tzMin = temp;
		}


		//Here i'm trying to get the largest min and the smallest max values from the potential intersections.
		double tMin;
		double tMax;

		if(txMin > tyMin)
		{
			tMin = txMin;
		}
		else
		{
			tMin = tyMin;
		}

		if(txMax < tyMax)
		{
			tMax = txMax;
		}
		else
		{
			tMax = tyMax;
		}


		if(txMin > tyMin || tyMin > txMax)
		{
			return false;
		}
		if(tMin > tzMax || tzMin > tMax)
		{
			return false;
		}
		if(tzMin > tMin) { tMin = tzMin; }
		if(tzMax < tMax) { tMax = tzMax; }
		
		//We're good so far.

		//tMin is the point of intersection on the ray! (as in the distance t along the ray) Hurray. we've intersected.

		
		return true;


		//NOTES: if tmin at the end is <= 0, the intersection began behind the origin. So will likely trigger if you're firing out from inside a bounding box... !!!
	}

	private:
		std::pair<Vector3, Vector3> minMaxCorners;
};


class Node
{
public:
	BoundingBox BoundingVolume;

	std::unique_ptr<Node> leftChild;
	std::unique_ptr<Node> rightChild;
	std::vector<Sphere*> nodeObjects;
};


class BoundingVolumeHierarchy
{
public:
	BoundingVolumeHierarchy() = default;
	~BoundingVolumeHierarchy() = default;

	std::unique_ptr<Node> thisNode = std::make_unique<Node>();

	void BuildBVH(Node* node, std::vector<Sphere*> objects);
	std::pair<Vector3, Vector3> calculateMinimumBox(std::vector<Sphere*> objects);
	void sortObjectsAlongLongestAxis(std::vector<Sphere*> objects);
	std::vector<Sphere*> splitObjects(std::vector<Sphere*> objects, LeftOrRight _leftOrRight);
};

