#pragma once
#include "Sphere.h"
#include "Vector3.h"
#include <vector>
#include <memory>
#include "Ray.h"

//https://coocoscode.wordpress.com/2010/12/15/building-bounding-volume-hiearchies/
//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection


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

	bool intersect(Ray& ray);

	private:
		std::pair<Vector3, Vector3> minMaxCorners;
};


class Node
{
public:
	BoundingBox boundingBox;
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
	void sortObjectsAlongAxis(std::vector<Sphere*> objects, int _int);
	std::pair<std::vector<Sphere*>, std::vector<Sphere*>> splitObjects(std::vector<Sphere*> objects, double & bestCost);
	double calculateCombinedSurfaceAreas(BoundingBox boxLeft, BoundingBox boxright);
	std::vector<Sphere*> GetObjectsUpToIndex(std::vector<Sphere*> _objects, int _upToIndex);
	std::vector<Sphere*> GetObjectsFromIndexToEnd(std::vector<Sphere*> objects, int _upFromIndex);
};

