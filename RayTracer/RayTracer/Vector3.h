#pragma once
#include <math.h>

class Vector3
{
public:
	Vector3();

	Vector3(double _x, double _y, double _z);
	Vector3(const Vector3 &p2) { x = p2.x; y = p2.y; z = p2.z; }

	~Vector3();

	double Distance(Vector3 posA, Vector3 posB);

	double GetX() const { return x; }
	double GetY() const { return y; }
	double GetZ() const { return z; }

	const Vector3 Up() { return { 0.0f, 1.0f, 0.0f }; }

	static Vector3 cross(Vector3 A, Vector3 B)
	{
		return Vector3(A.y * B.z - A.z * B.y,
			A.z * B.x - A.x * B.z,
			A.x * B.y - A.y * B.x);
	}

	//Operators//
	Vector3 operator * (const Vector3 &vec) const
	{
		return Vector3(x * vec.x, y * vec.y, z * vec.z);
	}

	Vector3 operator / (const double &d) const
	{
		return Vector3(x / d, y / d, z / d);
	}

	Vector3 operator / (const Vector3 &vec) const
	{
		return Vector3(x / vec.x, y / vec.y, z / vec.z);
	}

	friend Vector3 operator/ (const double d, const Vector3& vec)
	{
		return Vector3(d / vec.x, d / vec.y, d / vec.z);
	};

	Vector3 operator + (const double &d) const
	{
		return Vector3(x + d, y + d, z + d);
	}

	Vector3 operator+(const Vector3& vec) const
	{ 
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}
	Vector3 operator - (const Vector3 &vec) const
	{ 
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	Vector3 operator*(double b) const 
	{ 
		return Vector3(x*b, y*b, z*b); 
	}

	Vector3 operator % (Vector3& b) //THIS is a cross product
	{
		return Vector3(y*b.z - z * b.y, z*b.x - x * b.z, x*b.y - y * b.x);
	}





	Vector3 cross(const Vector3 &vec) const
	{
		return Vector3(	y * vec.z - z * vec.y,
						z * vec.x - x * vec.z,
						x * vec.y - y * vec.x	);
	}

	double dot(const Vector3 &b) const 
	{ 
		return x * b.x + y * b.y + z * b.z; 
	}

	double magnitude() const
	{
		return (double)sqrt(x*x + y * y + z * z);
	}

	Vector3& normalize() 
	{
		double inverseVal = 1 / magnitude();

		x *= inverseVal;
		y *= inverseVal;
		z *= inverseVal;

		return *this;
	}

	Vector3 multiplyBy(const Vector3 &b) const 
	{ 
		return Vector3(x*b.x, y*b.y, z*b.z); 
	}

	//Operators end//
	static double dot(const Vector3 &vecA, const Vector3 &vecB)
	{
		return (vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z);
	}

	double x, y, z = 0.0f;

private:

};

