#include "Vector3.h"
#include <math.h>



Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{

}


Vector3::~Vector3()
{

}

float Vector3::Distance(Vector3 posA, Vector3 posB)
{
	return 1.0f;
}
