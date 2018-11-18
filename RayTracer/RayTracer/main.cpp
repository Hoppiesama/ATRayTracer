// project based on smallpt, a Path Tracer by Kevin Beason, 2008. http://www.kevinbeason.com/smallpt/

#include <math.h>  
#include <stdlib.h> 
#include <stdio.h>  
#include <random>
#include <iostream>
#include <fstream>
#include "Camera.h"
#include "Sphere.h"
#include <atomic>
#include <string>

#include <thread>

#pragma region erand and Pi definitions
//Some initial changes were made to allow this to run/build on windows.
//sourced from https://drive.google.com/file/d/0B8g97JkuSSBwUENiWTJXeGtTOHFmSm51UC01YWtCZw/view

//Pi
double M_PI = 3.1415926;
// 1 over pi
double M_1_PI = 1.0 / M_PI;
//erand48 is not a part of c++ standard in windows, so an alternative...
double erand48(unsigned short xSubI[3])
{
	return (double)rand() / (double)RAND_MAX;
}
#pragma endregion


//TODO - Create Material Class and simply the radiance calls to only calculate diffuse. Keep it simple for week 2.
//TODO - Create/modify the "light" class and maybe implement "shadow" rays to get that stage done.


Sphere spheres[] = 
{
	//Scene: radius, position, emission, color, material 
	Sphere(1e5, Vector3(1e5 + 1 - 50 ,40.8,81.6), Vector3(),Vector3(.75,.25,.25),DIFF),//Left 
	Sphere(1e5, Vector3(-1e5 + 99 - 50,40.8,81.6),Vector3(),Vector3(.25,.25,.75),DIFF),//Rght 
	Sphere(1e5, Vector3(50 - 50,40.8, 1e5),     Vector3(),Vector3(.75,.75,.75),DIFF),//Back 
	//Sphere(1e5, Vec(50 -50,40.8,-1e5 + 170), Vec(),Vec(),           DIFF),//Frnt 
	//Sphere(1e5, Vec(50 -50, 1e5, 81.6),    Vec(),Vec(.75,.75,.75),DIFF),//Botm 
	//Sphere(1e5, Vec(50-50,-1e5 + 81.6,81.6),Vec(),Vec(.75,.75,.75),DIFF),//Top 
	Sphere(16.5,Vector3(27 - 50,16.5,47),       Vector3(),Vector3(1,1,1)*.999, SPEC),//Mirr 
	Sphere(16.5,Vector3(73 - 50,16.5,78),       Vector3(),Vector3(1,1,1)*.999, REFR),//Glas 
	Sphere(16.5, Vector3(0, 52, 15),Vector3(12,12,12),  Vector3(), DIFF) //Lite 

};

inline double clamp(double x) 
{ 
	if(x < 0)
	{
		return 0;
	}
	else if(x > 1)
	{
		return 1;
	}
	else
	{
		return x;
	}
}

// Gamma correction: https://learnopengl.com/Advanced-Lighting/Gamma-Correction
inline int toGammaCorrectedInt(double x) 
{ 
	return int(pow(clamp(x), 1 / 2.2) * 255 + .5); 
}

inline bool intersect(const Ray &r, double &t, int &id)
{
	double n = sizeof(spheres) / sizeof(Sphere), d, inf = t = 1e20;
	for(int i = int(n); i--;) if((d = spheres[i].intersect(r)) && d<t) { t = d; id = i; }
	return t<inf;
}

Vector3 radiance(const Ray &r, int depth, unsigned short *xSubi)
{
	double t;                   // distance to intersection 
	int id = 0;					// id of intersected object 
	if(!intersect(r, t, id))
	{
		return Vector3(); // if miss, return black 
	}
	const Sphere &hitObj = spheres[id];        // the hit object 

	Vector3 hitPoint = r.GetOrigin() + r.GetDirection()*t;
	Vector3 normal = (hitPoint - hitObj.position).normalize();
	Vector3 nl = normal.dot(r.GetDirection()) < 0 ? normal : normal * -1;
	Vector3 objectColour = hitObj.colour;

	//double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max refl 

	double p;
	// max refl 
	if(objectColour.x > objectColour.y && objectColour.x > objectColour.z)
	{
		p = objectColour.x;
	}
	else if(objectColour.y > objectColour.z)
	{
		p = objectColour.y;
	}
	else
	{
		p = objectColour.z;
	}

	//After 5 radiance recursions, uses random 
	//chance to end based on highest value in object colour
	if(++depth > 5)
	{
		if(erand48(xSubi) < p)
		{
			objectColour = objectColour * (1 / p);
		}
		else
		{
			return hitObj.emission; //R.R. 
		}
	}

	if(hitObj.refl == DIFF)
	{                  // Ideal DIFFUSE reflection 
		double r1 = 2 * M_PI*erand48(xSubi);
		double r2 = erand48(xSubi);
		double r2s = sqrt(r2);

		Vector3 w = nl;
		Vector3 u;

		//I THINK this is for determining whether a surface deflects light or not

		if(fabs(w.x) > 0.1)
		{
			u = Vector3(0, 1, 0).cross(w);
		}
		else
		{
			u = Vector3(1, 0, 0).cross(w);
		}
		u.normalize();
		//Vector3 u = ( (			fabs(w.x) > .1 ? Vector3(0, 1, 0) : Vector3(1, 0, 0)	) % w		).normalize();

		Vector3 v = w.cross(u); // % = cross

		// TOM!!! - is this an implementation of the monte carlo thing?

		//New direction randomised slightly due to diffuse colour.
		// u * cos(randomNo betwen 0 and 2Pi) + v * sin(same randomNo between 0 and 2Pi) + w * sqrt(1 - randomNo between 0.0 & 1.0) . normalized
		Vector3 newDirection = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();

		return hitObj.emission + objectColour.multiplyBy(radiance(Ray(hitPoint, newDirection), depth, xSubi));
	}
	else if(hitObj.refl == SPEC)            // Ideal SPECULAR reflection 
	{
		return hitObj.emission + objectColour.multiplyBy(radiance(Ray(hitPoint, r.GetDirection() - normal * 2 * normal.dot(r.GetDirection())), depth, xSubi));
	}

	// Ideal dielectric REFRACTION 
	Ray reflRay(hitPoint, r.GetDirection() - normal * 2 * normal.dot(r.GetDirection()));

	// Check if Ray is from the outside going in? 
	bool into = normal.dot(nl) > 0;

	double nc = 1;
	double nt = 1.5;
	double nnt;

	if(into)
	{
		nnt = nc / nt; // == 1/1.5 == 0.666666
	}
	else
	{
		nnt = nt / nc;  // == 1.5/1 == 1.5
	}

	double rayDirectionDotNormal = r.GetDirection().dot(nl);

	//TOM - Calculate angle of ray relative to surface, to determine if it should be total internal reflection?
	double cos2t = 1 - nnt * nnt * (1 - rayDirectionDotNormal * rayDirectionDotNormal);

	// Total internal reflection 
	if(cos2t < 0)
	{
		return hitObj.emission + objectColour.multiplyBy(radiance(reflRay, depth, xSubi));
	}
		
	Vector3 tdir;
	if(into)
	{
		tdir = r.GetDirection()*nnt - normal * ( 1 *(rayDirectionDotNormal*nnt + sqrt(cos2t)));
	}
	else
	{
		tdir = r.GetDirection()*nnt - normal * ( -1 * (rayDirectionDotNormal*nnt + sqrt(cos2t)));
	}
	tdir.normalize();

	double a = nt - nc; // == 1.5 - 1 == 0.5
	double b = nt + nc; // == 1.5 + 1 = 2.5
	double R0 = (a * a) / (b*b); // == 0.5^2 / (b^2)

	double c;
	if(into)
	{
		c = 1 - (-rayDirectionDotNormal);
	}
	else
	{
		c = tdir.dot(normal);
	}

	double Re = R0 + (1 - R0)*c*c*c*c*c;
	double Tr = 1 - Re;
	double P = .25 + .5*Re;
	double RP = Re / P;
	double TP = Tr / (1 - P);

	if(depth > 2)
	{
		if(erand48(xSubi) < P)
		{
			return hitObj.emission + objectColour.multiplyBy(radiance(reflRay, depth, xSubi)*RP);
		}
		else
		{
			return hitObj.emission + objectColour.multiplyBy(radiance(Ray(hitPoint, tdir), depth, xSubi)*TP);
		}
	}
	else
	{
		return hitObj.emission + objectColour.multiplyBy((radiance(reflRay, depth, xSubi)*Re + radiance(Ray(hitPoint, tdir), depth, xSubi)*Tr) );
	}
}



void threadOver(int samples, double fov, int yStart, int yEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera _cam, std::atomic<int>* _counter, std::string* _string)
{
	Vector3 cx = _cam.camRight * fov;
	//preserve aspect ration based on camRight
	Vector3 cy = (cx % _cam.lookDirection) * height / width;

	Vector3 r;

	for(int y = yStart; y<yEnd; y++)
	{                       // Loop over image rows 
		unsigned short Xi[3] = { 0,0, y*y*y };

		for(unsigned short x = 0; x < width; x++) // Loop cols 
		{
			int i = (height - y - 1)* width + x;

			//TODO - remove sub-pixel sampling.
			//TODO - remove multi-sampling.

			int result1 =  (int)( ( (float)(y- yStart) / (float)(yEnd - yStart) * 100) );
			int result2 = (int)( ( (float)x / (float)width) * 100);

			(*_string) = std::to_string(result1) + "." + std::to_string(result2);

			for(int subPixelY = 0; subPixelY < 2; subPixelY++)     // 2x2 subpixel rows 
			{
				for(int subPixelX = 0; subPixelX<2; subPixelX++, r = Vector3()) // 2x2 subpixel cols 
				{
					for(int s = 0; s<samples; s++)
					{
						//Produces random number between -1 and 1. Tent filter (weights towards centre point)
						double r1 = 2 * erand48(Xi);
						double dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double r2 = 2 * erand48(Xi);
						double dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);

						//Get the ray direction based on the offsets produced beforehand
						Vector3 direction = cx * (((subPixelX + .5 + dx) / 2 + x) / width - .5)
							+ cy * (((subPixelY + .5 + dy) / 2 + y) / height - .5)
							+ _cam.lookDirection;
						//push the offsets on the camera's "forward"

						r = r + radiance(Ray(_cam.position, direction.normalize()), 0, Xi)*(1. / samples);
					}
					(pixelColour)->at(i) = (pixelColour)->at(i) + Vector3(clamp(r.x), clamp(r.y), clamp(r.z))*.25;
				}
			}
		}
		
	}
	(*_counter)++;
}


int main(int argc, char *argv[])
{
	int width = 1024, height = 768, samps = argc == 2 ? atoi(argv[1]) / 4 : 1; // # samples 

	samps = 8; //override sample!

	Camera cam(Vector3(0, 52, 240), Vector3(0, 10, -1)); // cam pos, position to look at
	
	bool threaded = true;

	//Field of view multiplier used in testing. it's a simple value rather than a "degrees of camera aperture".
	//Needs additional work.
	double fov = 1.0;

	//used for output;
	std::vector<Vector3> pixelColour;
	pixelColour.resize(width*height);

	if(threaded)
	{
		std::string t1 = "";
		std::string t2 = "";
		std::string t3 = "";
		std::string t4 = "";

		std::atomic<int> finishedThreads = 0;

		std::thread thread = std::thread(threadOver, samps, fov, 0, height / 4, width, height, &pixelColour, cam, &finishedThreads, &t1);
		std::thread thread2 = std::thread(threadOver, samps, fov, height / 4, (height / 4) * 2, width, height, &pixelColour, cam, &finishedThreads, &t2);
		std::thread thread3 = std::thread(threadOver, samps, fov, (height / 4) * 2, (height / 4) * 3, width, height, &pixelColour, cam, &finishedThreads, &t3);
		std::thread thread4 = std::thread(threadOver, samps, fov, (height / 4) * 3, height, width, height, &pixelColour, cam, &finishedThreads, &t4);

		while(finishedThreads < 4)
		{
			std::cout << "\rComplete thr: " << finishedThreads;
			std::cout << "\tThr 1: " + t1;
			std::cout << "\tThr 2: " + t2;
			std::cout << "\tThr 3: " + t3;
			std::cout << "\tThr 4: " + t4;
		}

		//Added as a precaution
		thread.join();
		thread2.join();
		thread3.join();
		thread4.join();
	}
	else
	{
		cam.camRight = cam.lookDirection % cam.up;

		Vector3 cx = cam.camRight * fov;
		//preserve aspect ration based on camRight
		Vector3 cy = (cx % cam.lookDirection) * height / width;

		Vector3 r;

		for(int y = 0; y<height; y++)
		{                       
			// Loop over image rows 
			fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100.*y / (height - 1));
			unsigned short Xi[3] = { 0,0, y*y*y };

			for(unsigned short x = 0; x < width; x++) // Loop cols 
			{
				int i = (height - y - 1)* width + x;

				for(int subPixelY = 0; subPixelY < 2; subPixelY++)     // 2x2 subpixel rows 
				{
					for(int subPixelX = 0; subPixelX<2; subPixelX++, r = Vector3()) // 2x2 subpixel cols 
					{
						for(int s = 0; s<samps; s++)
						{
							//Produces random number between -1 and 1. Tent filter (weights towards centre point)
							double r1 = 2 * erand48(Xi);
							double dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
							double r2 = 2 * erand48(Xi);
							double dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);

							//Get the ray direction based on the offsets produced beforehand
							Vector3 direction = cx * (((subPixelX + .5 + dx) / 2 + x) / width - .5)
								+ cy * (((subPixelY + .5 + dy) / 2 + y) / height - .5)
								+ cam.lookDirection;
							//push the offsets on the camera's "forward"

							r = r + radiance(Ray(cam.position, direction.normalize()), 0, Xi)*(1. / samps);
						}
						pixelColour[i] = pixelColour[i] + Vector3(clamp(r.x), clamp(r.y), clamp(r.z))*.25;
					}
				}
			}
		}
	}

	std::ofstream output("output.ppm");
	output << "P3\n" << width << '\n' << height << '\n' << "255\n";
	for(int i = 0; i < width*height; i++)
	{
		output << toGammaCorrectedInt(pixelColour[i].x) << " " << toGammaCorrectedInt(pixelColour[i].y) << " " << toGammaCorrectedInt(pixelColour[i].z) << std::endl;
	}
}

