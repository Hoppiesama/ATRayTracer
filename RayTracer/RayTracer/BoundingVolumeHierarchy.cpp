#include "BoundingVolumeHierarchy.h"
#include <algorithm>


//TODO - change this to "objects" not sphere
#define nodeTraversalCost 3;
#define objectTraversalCost 5;


void BoundingVolumeHierarchy::BuildBVH(Node* node, std::vector<Object*> objects)
{
	node->boundingBox.SetBounds(calculateMinimumBox(objects));


	if(objects.size() <= 1)
	{
		node->nodeObjects.reserve(1);
		node->nodeObjects = std::move(objects);
		return;
	}

	std::vector<Object*> leftObjects;
	std::vector<Object*> rightObjects;

	double bestCost = DBL_MAX;
	double bestCostStore = bestCost;

	std::pair<std::vector<Object*>, std::vector<Object*>> bestPair;

	//0 == X, 1 == y, 2 == Z
	for (int i = 0; i < 3; i++)
	{
		sortObjectsAlongAxis(objects, i);
	//	findBestSurfaceArea
		std::pair<std::vector<Object*>, std::vector<Object*>> temp = splitObjects(objects, bestCost);

		if (bestCost < bestCostStore)
		{
			leftObjects = temp.first;
			rightObjects = temp.second;
			bestCostStore = bestCost;
		}
	}

	node->leftChild = std::make_unique<Node>();
	node->rightChild = std::make_unique<Node>();

	BuildBVH(node->leftChild.get(), leftObjects);
	BuildBVH(node->rightChild.get(), rightObjects);
}


std::pair<Vector3, Vector3> BoundingVolumeHierarchy::calculateMinimumBox(std::vector<Object*> objects)
{
	//Determine the centre point of all the objects.
	double x = 0;
	double y = 0;
	double z = 0;

	for(auto it = objects.begin(); it != objects.end(); it++)
	{
		x += (*it)->position.x;
		y += (*it)->position.y;
		z += (*it)->position.z;
	}

	Vector3 AveragedPosition(x / objects.size(), y / objects.size(), z / objects.size());


	//initiate bounding volume to smallest value above the averaged location.
	Vector3 bottomBackLeft(AveragedPosition.x - DBL_EPSILON, AveragedPosition.y -DBL_EPSILON, AveragedPosition.z -DBL_EPSILON);
	Vector3 topFrontRight(AveragedPosition.x +  DBL_EPSILON, AveragedPosition.y + DBL_EPSILON, AveragedPosition.z + DBL_EPSILON);


	for(auto it = objects.begin(); it != objects.end(); it++)
	{
		if((*it)->GetLowestXVert() < bottomBackLeft.x)
		{
			bottomBackLeft.x = (*it)->GetLowestXVert();
		}

		if((*it)->GetLowestYVert() < bottomBackLeft.y)
		{
			bottomBackLeft.y = (*it)->GetLowestYVert();
		}
		if((*it)->GetLowestZVert() < bottomBackLeft.z)
		{
			bottomBackLeft.z = (*it)->GetLowestZVert();
		}

		if((*it)->GetHighestXVert() > topFrontRight.x)
		{
			topFrontRight.x = (*it)->GetHighestXVert();
		}

		if((*it)->GetHighestYVert() > topFrontRight.y)
		{
			topFrontRight.y = (*it)->GetHighestYVert();
		}
		if((*it)->GetHighestZVert() > topFrontRight.z)
		{
			topFrontRight.z = (*it)->GetHighestZVert();
		}
	}

	return std::make_pair(bottomBackLeft, topFrontRight);
}

//Sorts by longest extent of the current node.
void BoundingVolumeHierarchy::sortObjectsAlongAxis(std::vector<Object*>& objects, int _int)
{	
	Vector3 extents = thisNode->boundingBox.GetExtents();

	if(_int == 0)
	{
		//Sort by x pos
		std::sort(objects.begin(), objects.end(), [](Object* a, Object* b) { return a->position.x < b->position.x; });
	}
	else if(_int == 1)
	{
		//Sort by y pos
		std::sort(objects.begin(), objects.end(), [](Object* a, Object* b) { return a->position.y < b->position.y; });
	}
	else
	{
		//Sort by z pos
		std::sort(objects.begin(), objects.end(), [](Object* a, Object* b) { return a->position.z < b->position.z; });
	}
}

//TODO - find the bounding box combinations with the lowest surface area
//std::pair<std::vector<Object*>, std::vector<Object*>> BoundingVolumeHierarchy::splitObjects(std::vector<Object*> objects, double& bestCost)
//{
//	int permutationsLeft = 0;
//	int permutationsRight = (int)objects.size();
//
//	for (int indexAtEndOfFirstSet = 0; indexAtEndOfFirstSet < (int)objects.size(); indexAtEndOfFirstSet++)
//	{
//
//		std::vector<Object*> leftOfSplit = GetObjectsUpToIndex(objects, indexAtEndOfFirstSet);
//		std::vector<Object*> rightOfSplit = GetObjectsFromIndexToEnd(objects, indexAtEndOfFirstSet);
//
//		BoundingBox boxLeft;
//		boxLeft.SetBounds(calculateMinimumBox(leftOfSplit));
//
//		BoundingBox boxRight;
//		boxRight.SetBounds(calculateMinimumBox(rightOfSplit));
//
//
//		double thisCost = calculateCombinedSurfaceAreas(boxLeft, boxRight);
//
//		if (thisCost < bestCost)
//		{
//			bestCost = thisCost;
//			permutationsLeft = indexAtEndOfFirstSet;
//		}
//	}
//
//	return std::make_pair(GetObjectsUpToIndex(objects, permutationsLeft), GetObjectsFromIndexToEnd(objects, permutationsLeft));
//}

std::pair<std::vector<Object*>, std::vector<Object*>> BoundingVolumeHierarchy::splitObjects(std::vector<Object*> objects, double& bestCost)
{
	int permutationsLeft = (int)((double)objects.size() * 0.5);

	if(permutationsLeft == 1 && objects.size() == 2)
	{
		permutationsLeft = 0;
	}

	//Do for loop over objects

	//getting each split permutation from 1 - 9, 2-8, 3-7 etc.
	//track best cost as (i * leftGroup.Area() ) + (objectsList.size() - i) * rightGroup.Area() )) DIVIDED BY parent area. 

	std::vector<Object*> leftOfSplit = GetObjectsUpToIndex(objects, permutationsLeft);
	std::vector<Object*> rightOfSplit = GetObjectsFromIndexToEnd(objects, permutationsLeft);

	BoundingBox boxLeft;
	boxLeft.SetBounds(calculateMinimumBox(leftOfSplit));

	BoundingBox boxRight;
	boxRight.SetBounds(calculateMinimumBox(rightOfSplit));


	double thisCost = calculateCombinedSurfaceAreas(boxLeft, boxRight);

	if(thisCost < bestCost)
	{
		bestCost = thisCost;
	}

	return std::make_pair(GetObjectsUpToIndex(objects, permutationsLeft), GetObjectsFromIndexToEnd(objects, permutationsLeft));
}

double BoundingVolumeHierarchy::calculateCombinedSurfaceAreas(BoundingBox boxLeft, BoundingBox boxRight)
{
	//get absolute differences between x, y and z as a vector.
	Vector3 extentsA = boxLeft.GetExtents();
	Vector3 extentsB = boxRight.GetExtents();

	Vector3 extentsParent = thisNode->boundingBox.GetExtents();


	double surfAreaA = 2 * (extentsA.x * extentsA.y) + 2 * (extentsA.x * extentsA.z) + 2 * (extentsA.y * extentsA.z);
	double surfAreaB = 2 * (extentsB.x * extentsB.y) + 2 * (extentsB.x * extentsB.z) + 2 * (extentsB.y * extentsB.z);

	double surfAreaParent = 2 * (extentsParent.x * extentsParent.y) + 2 * (extentsParent.x * extentsParent.z) + 2 * (extentsParent.y * extentsParent.z);

	return objectTraversalCost + (surfAreaA / surfAreaParent) * nodeTraversalCost + (surfAreaB / surfAreaParent) * nodeTraversalCost;
}

std::vector<Object*> BoundingVolumeHierarchy::GetObjectsUpToIndex(std::vector<Object*> _objects, int _upToIndex)
{
	std::vector<Object*> temp;
	for (int i = 0; i <= _upToIndex; i++)
	{
		temp.push_back(&(*_objects.at(i) ) );
	}

	return temp;
}

std::vector<Object*> BoundingVolumeHierarchy::GetObjectsFromIndexToEnd(std::vector<Object*> _objects, int _upFromIndex)
{
	std::vector<Object*> temp;
	for (int i = _upFromIndex +1; i <= _objects.size() - 1; i++)
	{
		temp.push_back(&(*_objects.at(i) ));
	}
	return temp;
}




//TODO - implement tree traversal through intersections
void BoundingVolumeHierarchy::intersectTree(Ray& _r, Node* _node, std::vector<Object*>& _objectsHit)
{
	if(_node->boundingBox.intersect(_r))
	{
		if(_node->nodeObjects.size() == 1)
		{
			_objectsHit.push_back(_node->nodeObjects.front() );
		}
		else
		{
			if(_node->leftChild != nullptr)
			{
				intersectTree(_r, _node->leftChild.get(), _objectsHit);
			}

			if(_node->rightChild != nullptr)
			{
				intersectTree(_r, _node->rightChild.get(), _objectsHit);
			}
		}
	}
}



bool BoundingBox::intersect(Ray & ray)
	{
		Vector3 rayDirection = ray.GetDirection();

		//Get the minimum and maximum that t can be based on this intersectioning concept.
		//https://www.scratchapixel.com/images/upload/ray-simple-shapes/2dfig.png


		double tmin = (minMaxCorners.first.x - ray.GetOrigin().x) / rayDirection.x;
		double tmax = (minMaxCorners.second.x - ray.GetOrigin().x) / rayDirection.x;


		if(tmin > tmax) std::swap(tmin, tmax);

		double tymin = (minMaxCorners.first.y - ray.GetOrigin().y) / rayDirection.y;
		double tymax = (minMaxCorners.second.y - ray.GetOrigin().y) / rayDirection.y;

		if(tymin > tymax) std::swap(tymin, tymax);

		if((tmin > tymax) || (tymin > tmax))
			return false;

		if(tymin > tmin)
			tmin = tymin;

		if(tymax < tmax)
			tmax = tymax;

		double tzmin = (minMaxCorners.first.z - ray.GetOrigin().z) / rayDirection.z;
		double tzmax = (minMaxCorners.second.z - ray.GetOrigin().z) / rayDirection.z;

		if(tzmin > tzmax) std::swap(tzmin, tzmax);

		if((tmin > tzmax) || (tzmin > tmax))
			return false;

		if(tzmin > tmin)
			tmin = tzmin;

		if(tzmax < tmax)
			tmax = tzmax;

		return true;
	}
