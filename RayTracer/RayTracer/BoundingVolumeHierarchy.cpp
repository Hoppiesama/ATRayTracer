#include "BoundingVolumeHierarchy.h"
#include <algorithm>


//TODO - change this to "objects" not sphere



void BoundingVolumeHierarchy::BuildBVH(Node* node, std::vector<Sphere*> objects)
{
	node->BoundingVolume.SetBounds(calculateMinimumBox(objects));


	if(objects.size() <= 2)
	{
		node->nodeObjects.reserve(2);
		node->nodeObjects = std::move(objects);
		return;
	}

	sortObjectsAlongLongestAxis(objects);

	std::vector<Sphere*> leftObjects = splitObjects(objects, LEFT);
	std::vector<Sphere*> rightObjects = splitObjects(objects, RIGHT);

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
void BoundingVolumeHierarchy::sortObjectsAlongLongestAxis(std::vector<Sphere*> objects)
{	

	Vector3 extents = thisNode->BoundingVolume.GetExtents();

	if(extents.x > extents.y && extents.x > extents.z)
	{
		//Sort by x pos
		std::sort(objects.begin(), objects.end(), [](Sphere* a, Sphere* b) { return a->position.x < b->position.x; });
	}
	else if(extents.y > extents.z)
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

//TODO - split objects down the middle, along X, or Y, or Z.
std::vector<Sphere*> BoundingVolumeHierarchy::splitObjects(std::vector<Sphere*> objects, LeftOrRight _leftOrRight)
{
	if(_leftOrRight == LEFT)
	{

	}
	else //right
	{

	}

	return objects;
}