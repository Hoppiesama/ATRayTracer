#pragma once
#include <math.h>

class Vector3
{
public:
	Vector3();

	Vector3(float _x, float _y, float _z);
	Vector3(const Vector3 &p2) { x = p2.x; y = p2.y; z = p2.z; }

	~Vector3();

	float Distance(Vector3 posA, Vector3 posB);

	float GetX() const { return x; }
	float GetY() const { return y; }
	float GetZ() const { return z; }

	//Operators//
	Vector3 operator * (const float &d) const
	{
		return Vector3(x * d, y * d, z * d);
	}

	Vector3 operator * (const Vector3 &vec) const
	{
		return Vector3(x * vec.x, y * vec.y, z * vec.z);
	}

	Vector3 operator / (const float &d) const
	{
		return Vector3(x / d, y / d, z / d);
	}

	Vector3 operator / (const Vector3 &vec) const
	{
		return Vector3(x / vec.x, y / vec.y, z / vec.z);
	}

	Vector3 operator + (const float &d) const
	{
		return Vector3(x + d, y + d, z + d);
	}

	Vector3 operator + (const Vector3 &vec) const
	{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3 operator - (const float &d) const
	{
		return Vector3(x - d, y - d, z - d);
	}

	Vector3 operator - (const Vector3 &vec) const
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	float magnitude() const
	{
		return (float)sqrt(x*x + y*y + z*z);
	}

	Vector3 cross(const Vector3 &vec) const
	{
		return Vector3(	y * vec.z - z * vec.y,
						z * vec.x - x * vec.z,
						x * vec.y - y * vec.x	);
	}

	Vector3& normalize() //make_unit_vector()
	{
		float inverseVal = 1 / magnitude();

		x *= inverseVal;
		y *= inverseVal;
		z *= inverseVal;

		return *this;
	}

	//Operators end//
	static float dot(const Vector3 &vecA, const Vector3 &vecB)
	{
		return (vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z);
	}

private:
	float x, y, z = 0.0f;
};

