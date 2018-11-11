#pragma once
#include "Vector3.h"
#include "Sphere.h"
#include "Ray.h"
#include "Camera.h"
#include <fstream>
#include <iostream>

struct tempColour
{
	float r, g, b = 0.0f;
	tempColour() { r = g = b = 0.0f; }
	tempColour(float _r, float _g, float _b) { r = _r; g = _g; b = _b; }
};


//Source@ https://youtu.be/ARn_yhgk7aE

int main()
{
	const int width = 320;
	const int height = 180;

	std::ofstream output("output.ppm");
	output << "P3\n" << width << '\n' << height << '\n' << "255\n";


	tempColour white(255, 255, 255);
	tempColour sphereColour(15, 15, 15);

	//Set sphere position as in the centre of the screen.
	Sphere mySphere(Vector3(width / 2, height /2, 0.0f), 20.0f);

	//Set up camera for later, currently it's looking at the origin but is unused!
	Camera cam(Vector3(0.0f, 2.0f, -5.0f), Vector3(0.0f, 0.0f, 0.0f));

	tempColour pixels[height][width];

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			//Ray from object to eyes, kind of a backwards ray trace to start. Will be changed as things get more complex.
			Ray ray(Vector3(x, y, 0.0f), Vector3(0.0f, 0.0f, 1.0f));

			float t = 20000.0f;

			if(mySphere.isHit(ray, t))
			{
				pixels[y][x] = white;
			}

			output << (int)pixels[y][x].r << std::endl;
			output << (int)pixels[y][x].g << std::endl;
			output << (int)pixels[y][x].b << std::endl;

			//std::cout << "Pixel: " << pixels[y][x].r << pixels[y][x].g << pixels[y][x].b << std::endl;
		}
	}

	return 0;
}