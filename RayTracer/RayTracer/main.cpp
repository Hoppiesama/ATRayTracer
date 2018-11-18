
// project based on smallpt, a Path Tracer by Kevin Beason, 2008. 

#include <math.h>  
#include <stdlib.h> 
#include <stdio.h>  
#include <random>
#include <iostream>
#include <fstream>
#include "Camera.h"
#include "Sphere.h"

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



Sphere spheres[] = {//Scene: radius, position, emission, color, material 
	Sphere(1e5, Vector3(1e5 + 1 - 50 ,40.8,81.6), Vector3(),Vector3(.75,.25,.25),DIFF),//Left 
	//Sphere(1e5, Vec(-1e5 + 99 - 50,40.8,81.6),Vec(),Vec(.25,.25,.75),DIFF),//Rght 
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

inline int toInt(double x) 
{ 
	return int(pow(clamp(x), 1 / 2.2) * 255 + .5); 
}

inline bool intersect(const Ray &r, double &t, int &id)
{
	double n = sizeof(spheres) / sizeof(Sphere), d, inf = t = 1e20;
	for(int i = int(n); i--;) if((d = spheres[i].intersect(r)) && d<t) { t = d; id = i; }
	return t<inf;
}

Vector3 radiance(const Ray &r, int depth, unsigned short *Xi)
{
	double t;                               // distance to intersection 
	int id = 0;                               // id of intersected object 
	if(!intersect(r, t, id)) return Vector3(); // if miss, return black 

	const Sphere &obj = spheres[id];        // the hit object 

	Vector3 hitPoint = r.GetOrigin() + r.GetDirection()*t;
	Vector3 normal = (hitPoint - obj.position).normalize();
	Vector3 nl = normal.dot(r.GetDirection()) < 0 ? normal : normal * -1;
	Vector3 f = obj.colour;

	//double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max refl 

	double p;
	// max refl 
	if(f.x > f.y && f.x > f.z)
	{
		p = f.x;
	}
	else if(f.y > f.z)
	{
		p = f.y;
	}
	else
	{
		p = f.z;
	}

	//After 5 radiance recursions, uses random 
	//chance to end based on highest value in object colour
	if(++depth > 5)
	{
		if(erand48(Xi) < p)
		{
			f = f * (1 / p);
		}
		else
		{
			return obj.emission; //R.R. 
		}
	}

	if(obj.refl == DIFF)
	{                  // Ideal DIFFUSE reflection 
		double r1 = 2 * M_PI*erand48(Xi);
		double r2 = erand48(Xi);
		double r2s = sqrt(r2);

		Vector3 w = nl;
		Vector3 u = ((fabs(w.x) > .1 ? Vector3(0, 1, 0) : Vector3(1, 0, 0)) % w).normalize();
		Vector3 v = w.cross(u); // % = cross
		Vector3 d = (u*cos(r1)*r2s + v * sin(r1)*r2s + w * sqrt(1 - r2)).normalize();

		return obj.emission + f.mult(radiance(Ray(hitPoint, d), depth, Xi));
	}
	else if(obj.refl == SPEC)            // Ideal SPECULAR reflection 
	{
		return obj.emission + f.mult(radiance(Ray(hitPoint, r.GetDirection() - normal * 2 * normal.dot(r.GetDirection())), depth, Xi));
	}
		
	// Ideal dielectric REFRACTION 
	Ray reflRay(hitPoint, r.GetDirection() - normal * 2 * normal.dot(r.GetDirection()));     

	// Ray from outside going in? 
	bool into = normal.dot(nl) > 0;                

	double nc = 1, nt = 1.5;
	double nnt = into ? nc / nt : nt / nc;
	double ddn = r.GetDirection().dot(nl);
	double cos2t = 1 - nnt * nnt*(1 - ddn * ddn);

	// Total internal reflection 
	if(cos2t < 0)
	{
		return obj.emission + f.mult(radiance(reflRay, depth, Xi));
	}
		
	Vector3 tdir = (r.GetDirection()*nnt - normal * ((into ? 1 : -1)*(ddn*nnt + sqrt(cos2t)))).normalize();

	double a = nt - nc;
	double b = nt + nc;
	double R0 = a * a / (b*b);
	double c = 1 - (into ? -ddn : tdir.dot(normal));

	double Re = R0 + (1 - R0)*c*c*c*c*c;
	double Tr = 1 - Re;
	double P = .25 + .5*Re;
	double RP = Re / P;
	double TP = Tr / (1 - P);

	return obj.emission + f.mult(depth>2 ? (erand48(Xi)<P ?   // Russian roulette 
		radiance(reflRay, depth, Xi)*RP : radiance(Ray(hitPoint, tdir), depth, Xi)*TP) :
		radiance(reflRay, depth, Xi)*Re + radiance(Ray(hitPoint, tdir), depth, Xi)*Tr);
}




int main(int argc, char *argv[])
{
	int width = 1024, height = 768, samps = argc == 2 ? atoi(argv[1]) / 4 : 1; // # samples 

	samps = 4;
	Camera cam(Vector3(120, 52, 240), Vector3(0, 10, -1)); // cam pos, position to look at
	
	//Field of view multiplier used in testing. it's a simple value rather than a "degrees of camera aperture".
	//Needs additional work.
	double fov = 1;

	//double aspectRatio = height / width;

	//Right
	cam.camRight = cam.lookDirection % cam.up;
	Vector3 cx = cam.camRight * fov;
	//preserve aspect ration based on camRight
	Vector3 cy = (cx % cam.lookDirection) * height / width;

	Vector3 r;

	Vector3* pixelColour = new Vector3[width*height];

#pragma omp parallel for schedule(dynamic, 4) private(r)       // OpenMP 

	for(int y = 0; y<height; y++)
	{                       // Loop over image rows 
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100.*y / (height - 1));
		unsigned short Xi[3] = { 0,0, y*y*y };
		for(unsigned short x = 0; x < width; x++) // Loop cols 
		{
			int i = (height - y - 1)* width + x;

			//TODO - remove sub-pixel sampling.
			//TODO - remove multi-sampling.

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
							              + cam.lookDirection; //push the offsets on the camera's "forward"

						r = r + radiance(Ray(cam.position + direction * 140, direction.normalize()), 0, Xi)*(1. / samps);
						//r = r + radiance(Ray(cam.position, direction.normalize()), 0, Xi)*(1. / samps);
					} // Camera rays are pushed ^^^^^ forward to start in interior 
					pixelColour[i] = pixelColour[i] + Vector3(clamp(r.x), clamp(r.y), clamp(r.z))*.25;
				}
			}
		}
	}
	std::ofstream output("output.ppm");
	output << "P3\n" << width << '\n' << height << '\n' << "255\n";
	for(int i = 0; i<width*height; i++)
		output << toInt(pixelColour[i].x) << " " << toInt(pixelColour[i].y) << " " << toInt(pixelColour[i].z) << std::endl;
}

