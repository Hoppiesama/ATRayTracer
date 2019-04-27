#pragma once
#include <SFML/Graphics.hpp>
#include "Vector3.h"


class Texture
{
public:
	Texture() = default;
	~Texture() = default;

	void loadTexture(std::string _fileName)
	{
		textureData.loadFromFile(_fileName);
	}

	//TODO - confirm this can work
	Vector3 GetPixel(double _u, double _v)
	{
		float u = _u - (float)std::floor(_u);
		float v = _v - (float)std::floor(_v);
		u *= textureData.getSize().x - 1;
		v *= textureData.getSize().y - 1;

		sf::Color color = textureData.getPixel((int)u, (int)v);

		double r;
		double g;
		double b;

		r = color.r / 255.0;
		g = color.g / 255.0;
		b = color.b / 255.0;

		Vector3 vectorColour(r, g, b);
		return vectorColour;
	}

private:

	sf::Image textureData;
};

