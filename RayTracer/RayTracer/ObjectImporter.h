#pragma once
#include <string>
#include <vector>
#include "Vector3.h"
#include "VertexStructs.h"
#include "Model.h"

class ObjectImporter
{
public:
	ObjectImporter();
	~ObjectImporter();

	bool Import(std::string _fileName, Model* _object);

	void ConvertToVertexStructs(std::vector<Vertex3>& _vertices, std::vector<int>& _indices);

private:
	std::vector<std::string> split(const std::string &s, char delim);

	std::vector<int> indices;
	std::vector<int> vertexTextureIndices;
	std::vector<int> vertexNormalIndices;

	std::vector<Vector3> vertexPositions;
	std::vector<Vector3> vertexTextureUV;
	std::vector<Vector3> vertexNormals;
};

