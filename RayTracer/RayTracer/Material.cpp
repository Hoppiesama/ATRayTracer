#include "Material.h"

void Material::initTextureFromFile(std::string _fileName)
{
	textureData.loadTexture(_fileName);
	isTextured = true;
}
