#include "ObjectImporter.h"
#include <fstream>
#include <sstream>



ObjectImporter::ObjectImporter()
{
}


ObjectImporter::~ObjectImporter()
{
}

bool ObjectImporter::Import(std::string _fileName, Model* _object )
{
	std::string fileName = _fileName;

	/*switch(_shapeToLoad)
	{
	case GeomShapes::DOOR_WALL:
	{
		fileName = "Geometry/doorWall.obj";
		break;
	}
	case GeomShapes::ROOF_PYRAMIDHIP:
	{
		fileName = "Geometry/pyramidHip.obj";
		break;
	}
	case GeomShapes::ROOF_HIP:
	{
		fileName = "Geometry/hip.obj";
		break;
	}
	case GeomShapes::ROOF_CONE:
	{
		fileName = "Geometry/cone.obj";
		break;
	}
	case GeomShapes::WINDOW:
	{
		fileName = "Geometry/Window.obj";
		break;
	}
	case GeomShapes::FLAT:
	{
		fileName = "Geometry/Flat.obj";
		break;
	}
	case GeomShapes::DOOR:
	{
		fileName = "Geometry/Door.obj";
		break;
	}
	case GeomShapes::STAIRS:
	{
		fileName = "Geometry/stairs.obj";
		break;
	}
	case GeomShapes::ROOF_OPENGABLE:
	{
		fileName = "Geometry/openGable.obj";
		break;
	}
	}*/

	std::ifstream inFile(fileName);

	std::string line;

	while(std::getline(inFile, line))
	{
		std::stringstream lineStream{ line };
		//ignore comment lines

		std::string test;

		lineStream >> test;

		if(test == "v")
		{
			//deal in vertex positions

			Vector3 temp;
			lineStream >> temp.x >> temp.y >> temp.z;
			//temp.z *= -1.0f;
			vertexPositions.push_back(temp);
		}

		//Deal in vertex textures
		if(test == "vt")
		{
			Vector3 temp;
			lineStream >> temp.x >> temp.y;
			vertexTextureUV.push_back(temp);
		}
		//Deal in vertex normals
		if(test == "vn")
		{
			Vector3 temp;
			lineStream >> temp.x >> temp.y >> temp.z;
			//temp.z *= -1.0f;
			vertexNormals.push_back(temp);
		}

		if(test == "f")
		{
			std::string value1, value2, value3;

			lineStream >> value1 >> value2 >> value3;

			if(value1.find('/') != std::string::npos)
				;// find
			else
				;// not find

			std::vector<std::string> value1vec = split(value1, '/');
			std::vector<std::string> value2vec = split(value2, '/');
			std::vector<std::string> value3vec = split(value3, '/');

			indices.push_back((unsigned short)std::stoi(value1vec.front()));
			indices.push_back((unsigned short)std::stoi(value2vec.front()));
			indices.push_back((unsigned short)std::stoi(value3vec.front()));

			vertexTextureIndices.push_back((unsigned short)std::stoi(value1vec[1]));
			vertexTextureIndices.push_back((unsigned short)std::stoi(value2vec[1]));
			vertexTextureIndices.push_back((unsigned short)std::stoi(value3vec[1]));

			vertexNormalIndices.push_back((unsigned short)std::stoi(value1vec[2]));
			vertexNormalIndices.push_back((unsigned short)std::stoi(value2vec[2]));
			vertexNormalIndices.push_back((unsigned short)std::stoi(value3vec[2]));

		}

	}

	int index = 0;
	int duplicateIndex = 0;
	bool foundDuplicate = false;

	ConvertToVertexStructs(_object->vertices, _object->indices);

	/*switch(_shapeToLoad)
	{
	case GeomShapes::DOOR_WALL:
	{
		ConvertToVertexStructs(dataHolder.cubeVertices, dataHolder.cubeIndices);
		break;
	}

	case GeomShapes::DOOR:
	{
		ConvertToVertexStructs(dataHolder.doorVertices, dataHolder.doorIndices);
		break;
	}

	case GeomShapes::ROOF_PYRAMIDHIP:
	{
		ConvertToVertexStructs(dataHolder.pyramidHipVertices, dataHolder.pyramidHipIndices);
		break;
	}
	case GeomShapes::FLAT:
	{
		ConvertToVertexStructs(dataHolder.flatVertices, dataHolder.flatIndices);
		break;
	}
	case GeomShapes::WINDOW:
	{
		ConvertToVertexStructs(dataHolder.windowVertices, dataHolder.windowIndices);
		break;
	}
	case GeomShapes::ROOF_HIP:
	{
		ConvertToVertexStructs(dataHolder.hipVertices, dataHolder.hipIndices);
		break;
	}
	case GeomShapes::BB_CYLINDER:
	{
		ConvertToVertexStructs(dataHolder.cylinderVertices, dataHolder.cylinderIndices);
		break;
	}
	case GeomShapes::ROOF_CONE:
	{
		ConvertToVertexStructs(dataHolder.coneVertices, dataHolder.coneIndices);
		break;
	}
	case GeomShapes::STAIRS:
	{
		ConvertToVertexStructs(dataHolder.stairsVertices, dataHolder.stairsIndices);
		break;
	}
	case GeomShapes::ROOF_OPENGABLE:
	{
		ConvertToVertexStructs(dataHolder.openGableVertices, dataHolder.openGableIndices);
		break;
	}
	}*/
	indices.clear();
	vertexTextureIndices.clear();
	vertexNormalIndices.clear();

	vertexPositions.clear();
	vertexTextureUV.clear();
	vertexNormals.clear();

	return true;
}

void ObjectImporter::ConvertToVertexStructs(std::vector<Vertex3>& _vertices, std::vector<int>& _indices)
{
	int index = 0;
	int duplicateIndex = 0;
	bool foundDuplicate = false;

	for(unsigned int i = 0; i < indices.size(); i++)
	{
		Vertex3 nextVert = Vertex3{ vertexPositions[indices[i] - 1].x, vertexPositions[indices[i] - 1].y, vertexPositions[indices[i] - 1].z,
			vertexTextureUV[vertexTextureIndices[i] - 1].x,vertexTextureUV[vertexTextureIndices[i] - 1].y,
			vertexNormals[vertexNormalIndices[i] - 1].x,vertexNormals[vertexNormalIndices[i] - 1].y, vertexNormals[vertexNormalIndices[i] - 1].z };

		for(auto& vert : _vertices)
		{
			if(vert.CompareTo(nextVert))
			{
				foundDuplicate = true;
				break;
			}
			duplicateIndex++;
		}

		if(foundDuplicate)
		{
			_indices.emplace_back(duplicateIndex);
		}
		else
		{
			_vertices.emplace_back(nextVert);
			_indices.emplace_back(index);
			index++;
		}

		duplicateIndex = 0;
		foundDuplicate = false;
	}
}

//function courtesy of http://ysonggit.github.io/coding/2014/12/16/split-a-string-using-c.html
std::vector<std::string> ObjectImporter::split(const std::string &s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> tokens;
	while(getline(ss, item, delim))
	{
		tokens.push_back(item);
	}
	return tokens;
}