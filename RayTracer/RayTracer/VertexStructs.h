#pragma once
#include "Vector3.h"

struct Vertex3
{
	Vertex3() {}
	Vertex3(double x, double y, double z,
		double u, double v,
		double nx, double ny, double nz)
		: pos(x, y, z), texCoord(u, v, 0.0), normal(nx, ny, nz)
	{
	}

	Vector3 pos = { 0.0, 0.0, 0.0 };
	Vector3 texCoord = { 0.0, 0.0, 0.0 };
	Vector3 normal = { 0.0, 0.0, 0.0};

	bool CompareTo(Vertex3 _toComp)
	{
		if(pos.x == _toComp.pos.x &&
			pos.y == _toComp.pos.y &&
			pos.z == _toComp.pos.z &&
			texCoord.x == _toComp.texCoord.x &&
			texCoord.y == _toComp.texCoord.y &&
			normal.x == _toComp.normal.x &&
			normal.y == _toComp.normal.y &&
			normal.z == _toComp.normal.z)
		{
			return true;
		}

		return false;
	}
};