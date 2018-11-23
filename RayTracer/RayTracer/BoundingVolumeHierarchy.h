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
	std::vector<Object*> nodeObjects;
};


class BoundingVolumeHierarchy
{
public:
	BoundingVolumeHierarchy() = default;
	~BoundingVolumeHierarchy() = default;

	std::unique_ptr<Node> thisNode = std::make_unique<Node>();


	void BuildBVH(Node* node, std::vector<Object*> objects);

	std::pair<Vector3, Vector3> calculateMinimumBox(std::vector<Object*> objects);

	void sortObjectsAlongAxis(std::vector<Object*>& objects, int _int);

	std::pair<std::vector<Object*>, std::vector<Object*>> splitObjects(std::vector<Object*> objects, double & bestCost);

	double calculateCombinedSurfaceAreas(BoundingBox boxLeft, BoundingBox boxright);

	std::vector<Object*> GetObjectsUpToIndex(std::vector<Object*> _objects, int _upToIndex);

	std::vector<Object*> GetObjectsFromIndexToEnd(std::vector<Object*> objects, int _upFromIndex);

	void intersectTree(Ray& _r, Node* _node, std::vector<Object*>& _objectsHit);
};

