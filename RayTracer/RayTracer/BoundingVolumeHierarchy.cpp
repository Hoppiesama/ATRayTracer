#include "BoundingVolumeHierarchy.h"
#include <algorithm>


//TODO - change this to "objects" not sphere
#define nodeTraversalCost 3;
#define objectTraversalCost 5;


void BoundingVolumeHierarchy::BuildBVH(Node* node, std::vector<Sphere*> objects)
{
	node->boundingBox.SetBounds(calculateMinimumBox(objects));


	if(objects.size() <= 1)
	{
		node->nodeObjects.reserve(1);
		node->nodeObjects = std::move(objects);
		return;
	}

	int axisWithBestSurfaceArea;

	std::vector<Sphere*> leftObjects;
	std::vector<Sphere*> rightObjects;

	double bestCost = DBL_MAX;
	double bestCostStore = bestCost;

	std::pair<std::vector<Sphere*>, std::vector<Sphere*>> bestPair;

	//0 == X, 1 == y, 2 == Z
	for (int i = 0; i < 3; i++)
	{
		sortObjectsAlongAxis(objects, i);
	//	findBestSurfaceArea
		std::pair<std::vector<Sphere*>, std::vector<Sphere*>> temp = splitObjects(objects, bestCost);

		if (bestCost < bestCostStore)
		{
			leftObjects = temp.first;
			rightObjects = temp.second;
		}
	}

	node->leftChild = std::make_unique<Node>();
	node->rightChild = std::make_unique<Node>();

	BuildBVH(node->leftChild.get(), leftObjects);
	BuildBVH(node->rightChild.get(), rightObjects);
}


std::pair<Vector3, Vector3> BoundingVolumeHierarchy::calculateMinimumBox(std::vector<Sphere*> objects)
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

		if((*it)->GetHighestXVert() > topFrontRight.x)
		{
			topFrontRight.x = (*it)->GetHighestXVert();
		}

		if((*it)->GetLowestYVert() < bottomBackLeft.y)
		{
			bottomBackLeft.y = (*it)->GetLowestYVert();
		}

		if((*it)->GetHighestYVert() > topFrontRight.y)
		{
			topFrontRight.y = (*it)->GetHighestYVert();
		}

		if((*it)->GetLowestZVert() < bottomBackLeft.z)
		{
			bottomBackLeft.z = (*it)->GetLowestZVert();
		}

		if((*it)->GetHighestZVert() > topFrontRight.z)
		{
			topFrontRight.z = (*it)->GetHighestZVert();
		}
	}

	return std::make_pair(bottomBackLeft, topFrontRight);
}

//Sorts by longest extent of the current node.
void BoundingVolumeHierarchy::sortObjectsAlongAxis(std::vector<Sphere*>& objects, int _int)
{	
	Vector3 extents = thisNode->boundingBox.GetExtents();

	if(_int == 0)
	{
		//Sort by x pos
		std::sort(objects.begin(), objects.end(), [](Sphere* a, Sphere* b) { return a->position.x < b->position.x; });
	}
	else if(_int == 1)
	{
		//Sort by y pos
		std::sort(objects.begin(), objects.end(), [](Sphere* a, Sphere* b) { return a->position.y < b->position.y; });
	}
	else
	{
		//Sort by z pos
		std::sort(objects.begin(), objects.end(), [](Sphere* a, Sphere* b) { return a->position.z < b->position.z; });
	}
}

//TODO - find the bounding box combinations with the lowest surface area
std::pair<std::vector<Sphere*>, std::vector<Sphere*>> BoundingVolumeHierarchy::splitObjects(std::vector<Sphere*> objects, double& bestCost)
{
	int permutationsLeft = 0;
	int permutationsRight = (int)objects.size();

	for (int indexAtEndOfFirstSet = 0; indexAtEndOfFirstSet < (int)objects.size(); indexAtEndOfFirstSet++)
	{

		std::vector<Sphere*> leftOfSplit = GetObjectsUpToIndex(objects, indexAtEndOfFirstSet);
		std::vector<Sphere*> rightOfSplit = GetObjectsFromIndexToEnd(objects, indexAtEndOfFirstSet);

		BoundingBox boxLeft;
		boxLeft.SetBounds(calculateMinimumBox(leftOfSplit));

		BoundingBox boxRight;
		boxRight.SetBounds(calculateMinimumBox(rightOfSplit));


		double thisCost = calculateCombinedSurfaceAreas(boxLeft, boxRight);

		if (thisCost < bestCost)
		{
			bestCost = thisCost;
			permutationsLeft = indexAtEndOfFirstSet;
		}
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

std::vector<Sphere*> BoundingVolumeHierarchy::GetObjectsUpToIndex(std::vector<Sphere*> _objects, int _upToIndex)
{
	std::vector<Sphere*> temp;
	for (int i = 0; i <= _upToIndex; i++)
	{
		temp.push_back(&(*_objects.at(i) ) );
	}

	return temp;
}

std::vector<Sphere*> BoundingVolumeHierarchy::GetObjectsFromIndexToEnd(std::vector<Sphere*> _objects, int _upFromIndex)
{
	std::vector<Sphere*> temp;
	for (int i = _upFromIndex +1; i <= _objects.size() - 1; i++)
	{
		temp.push_back(&(*_objects.at(i) ));
	}
	return temp;
}




//TODO - implement tree traversal through intersections
Object * BoundingVolumeHierarchy::intersectTree(Ray & _r)
{
	Object* temp = nullptr;

	//thisNode->boundingBox.intersect(_r);

	return temp;
}

bool BoundingBox::intersect(Ray & ray)
	{
		Vector3 rayDirection = ray.GetDirection();

		//Get the minimum and maximum that t can be based on this intersectioning concept.
		//https://www.scratchapixel.com/images/upload/ray-simple-shapes/2dfig.png


		//Do it for x
		double txMin = (minMaxCorners.first.x - ray.GetOrigin().x) / rayDirection.x;
		double txMax = (minMaxCorners.second.x - ray.GetOrigin().x) / rayDirection.x;

		double temp;

		if (txMax < txMin)
		{
			temp = txMax;
			txMax = txMin;
			txMin = temp;
		}

		double tyMin = (minMaxCorners.first.y - ray.GetOrigin().y) / rayDirection.y;
		double tyMax = (minMaxCorners.second.y - ray.GetOrigin().y) / rayDirection.y;


		//then do it for y
		if (tyMax < tyMin)
		{
			temp = tyMax;
			tyMax = tyMin;
			tyMin = temp;
		}


		//then do it for z
		double tzMin = (minMaxCorners.first.z - ray.GetOrigin().z) / rayDirection.z;
		double tzMax = (minMaxCorners.second.z - ray.GetOrigin().z) / rayDirection.z;


		if (tzMax < tzMin)
		{
			temp = txMax;
			tzMax = tzMin;
			tzMin = temp;
		}


		//Here i'm trying to get the largest min and the smallest max values from the potential intersections.
		double tMin;
		double tMax;

		if (txMin > tyMin)
		{
			tMin = txMin;
		}
		else
		{
			tMin = tyMin;
		}

		if (txMax < tyMax)
		{
			tMax = txMax;
		}
		else
		{
			tMax = tyMax;
		}


		if (txMin > tyMin || tyMin > txMax)
		{
			return false;
		}
		if (tMin > tzMax || tzMin > tMax)
		{
			return false;
		}
		if (tzMin > tMin) { tMin = tzMin; }
		if (tzMax < tMax) { tMax = tzMax; }

		//We're good so far.

		//tMin is the point of intersection on the ray! (as in the distance t along the ray) Hurray. we've intersected.


		return true;


		//NOTES: if tmin at the end is <= 0, the intersection began behind the origin. So will likely trigger if you're firing out from inside a bounding box... !!!
	}
