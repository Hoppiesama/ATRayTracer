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
#include "BoundingVolumeHierarchy.h"

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
	////Scene: radius, position, emission, color, material 
	//Sphere(18, Vector3(100 + 1 - 50 ,40.8,81.6), Vector3(),Vector3(.75,.25,.25),DIFF),//Left 
	//Sphere(18, Vector3(-100 + 99 - 50,40.8,81.6),Vector3(),Vector3(.25,.25,.75),DIFF),//Rght 
	//Sphere(18, Vector3(50 - 50,40.8, 100),     Vector3(),Vector3(.75,.75,.75),DIFF),//Back 
	////Sphere(100, Vec(50 -50,40.8,-100 + 170), Vec(),Vec(),           DIFF),//Frnt 
	//Sphere(18, Vector3(50 -50, 100, 81.6),    Vector3(),Vector3(.75,.75,.75),DIFF),//Botm 
	//Sphere(18, Vector3(50-50,-100 + 81.6,81.6),Vector3(),Vector3(.75,.75,.75),DIFF),//Top 
	//Sphere(16.5,Vector3(27 - 50,16.5,47),       Vector3(),Vector3(1,1,1)*.999, SPEC),//Mirr 
	//Sphere(16.5,Vector3(73 - 50,16.5,78),       Vector3(),Vector3(1,1,1)*.999, REFR),//Glas 
	//Sphere(16.5, Vector3(0, 52, 15),Vector3(12,12,12),  Vector3(), DIFF) //Lite 

	//Scene: radius, position, emission, color, material 
	Sphere(1e5, Vector3(1e5 + 1 - 50 ,40.8,81.6), Vector3(),Vector3(.75,.25,.25),DIFF),//Left 
	Sphere(1e5, Vector3(-1e5 + 99 - 50,40.8,81.6),Vector3(),Vector3(.25,.25,.75),DIFF),//Rght 
	//Sphere(1e5, Vector3(50 - 50,40.8, 1e5),     Vector3(),Vector3(.75,.75,.75),DIFF),//Back 
	Sphere(1e5, Vector3(50 -50,40.8,-1e5 + 170), Vector3(),Vector3(),           DIFF),//Frnt 

	Sphere(1e5, Vector3(50 -50, 1e5, 81.6),    Vector3(),Vector3(.75,.75,.75),DIFF),//Botm 
	Sphere(1e5, Vector3(0.0, -1e5 + 81.6,81.6),Vector3(),Vector3(.75,.75,.75),DIFF),//Top 
	Sphere(16.5,Vector3(27 - 50,16.5,47),       Vector3(),Vector3(1,1,1)*.999, SPEC),//Mirr 
	Sphere(16.5,Vector3(73 - 50,16.5,78),       Vector3(),Vector3(1,1,1)*.999, REFR),//Glas 

	Sphere(16.5, Vector3(0.0, 50.0, 0.0),		Vector3(12,12,12),  Vector3(), DIFF), //Lite 

	Sphere(16.5,Vector3(12,16.5,47),       Vector3(),Vector3(.75,.25,.25)*.999, DIFF),//Mirr 
	Sphere(16.5,Vector3(24,6.5,12),       Vector3(),Vector3(.75,.25,.25)*.999, DIFF),//Glas 
	Sphere(16.5,Vector3(-12,11.5,47),       Vector3(),Vector3(.75,.25,.25)*.999, DIFF),//Mirr 
	Sphere(16.5,Vector3(-24 ,22,22),       Vector3(),Vector3(.75,.25,.25)*.999, DIFF),//Glas 
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
	double n = sizeof(spheres) / sizeof(Sphere);
	double d;
	double inf = t = 1e20;
	for(int i = int(n); i--;)
		if((d = spheres[i].intersect(r)) && d<t)
		{ t = d; id = i; }

	return t<inf;
}

inline bool intersect(const Ray &r, double &t, int &vectorIndex, std::vector<Object*> objects)
{
	//double n = sizeof(spheres) / sizeof(Sphere);
	double n = objects.size();
	double d;
	double inf = t = DBL_MAX;


	for(int i = (int)objects.size() - 1; i >= 0; i--)
	{
		d = objects.at(i)->intersect(r);

		if(d < t && d != 0.0)
		{ 
			t = d; 
			vectorIndex = i; 
		}
	}
	return t<inf;
}

Vector3 radiance(const Ray &r, int depth, unsigned short *xSubi, std::vector<Object*> objects, BoundingVolumeHierarchy* bvh)
{
	double t;                   // distance to intersection 
	int id = 0;					// id of intersected object 

	//TODO - replace the below with bvh.getObjectsToTest(Ray& theRay)
	//then run new version of "intersect" with the "id" being replaced by an Object* to be assigned the first object hit. ALSO, pass in the returned vector of objects from the bvh above.

	//if(!intersect(r, t, id))
	//{
	//	return Vector3();
	//}

	if(!intersect(r, t, id, objects))
	{
		return Vector3(0.0, 0.0,0.0); // if miss, return black 
	}

	//const Sphere &hitObj = spheres[id];        // the hit object 
	const Object* hitObj = objects.at(id);

	Vector3 hitPoint = r.GetOrigin() + r.GetDirection()*t;
	Vector3 normal = (hitPoint - hitObj->position).normalize();
	Vector3 nl = normal.dot(r.GetDirection()) < 0 ? normal : normal * -1;
	Vector3 objectColour = hitObj->colour;

	//TODO - remove this, it's super simple to test BVH
	//return hitObj->colour;

	//double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max refl 

	double probability;
	// Get the highest colour value to determine likelihood of continuing in russian roulette.
	if(objectColour.x > objectColour.y && objectColour.x > objectColour.z)
	{
		probability = objectColour.x;
	}
	else if(objectColour.y > objectColour.z)
	{
		probability = objectColour.y;
	}
	else
	{
		probability = objectColour.z;
	}

	//After 5 radiance recursions, uses random 
	//chance to end based on highest value in object colour
	if(++depth > 5)
	{
		//in the case that p = 0 (no colour value), it can't possibly trigger and emission is returned instead.
		if(erand48(xSubi) < probability)
		{
			objectColour = objectColour * (1 / probability);
		}
		else
		{
			return hitObj->emission; //R.R. 
		}
	}

	if(hitObj->refl == DIFF)
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

		Vector3 v = w.cross(u); // % = cross

		// TOM!!! - is this an implementation of the monte carlo thing?
		//New direction randomised slightly due to diffuse colour.
		// u * cos(randomNo betwen 0 and 2Pi) + v * sin(same randomNo between 0 and 2Pi) + w * sqrt(1 - randomNo between 0.0 & 1.0) . normalized
		Vector3 newDirection = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();

		Ray ray(hitPoint, newDirection);

		objects.clear();

		bvh->intersectTree(ray, bvh->thisNode.get(), objects);

		return hitObj->emission + objectColour.multiplyBy(radiance(ray, depth, xSubi, objects, bvh));
	}
	else if(hitObj->refl == SPEC)            // Ideal SPECULAR reflection 
	{

		Vector3 newDirection = r.GetDirection() - normal * 2 * normal.dot(r.GetDirection());

		Ray ray(hitPoint, newDirection);

		objects.clear();

		bvh->intersectTree(ray, bvh->thisNode.get(), objects);

		//r.GetDirection() - normal * 2 * normal.dot(r.GetDirection()) is a mirrored angle from the normal, e.g. angle of incidence and angle of reflection from the normal are the same.
		//based on the idea shown here https://youtu.be/ytRrjf9OPHg?t=1031 ...
		return hitObj->emission + objectColour.multiplyBy(radiance(ray, depth, xSubi, objects, bvh));
	}

	//dielectric REFRACTION 
	Ray reflRay(hitPoint, r.GetDirection() - normal * 2 * normal.dot(r.GetDirection()));

	// Check if Ray is from the outside going in? 
	bool into = normal.dot(nl) > 0;

	double airRefractionIndex = 1;

	//TODO - replace this with access to hit object's material "Index of Refraction"
	double glassRefractionIndex = 1.5;


	double nnt;

	//If going in, we calculate the change in angle due to the refraction. If coming out, it's inverted back.
	if(into)
	{
		nnt = airRefractionIndex / glassRefractionIndex; // == 1/1.5 == 0.666666
	}
	else
	{
		nnt = glassRefractionIndex / airRefractionIndex;  // == 1.5/1 == 1.5
	}



	double rayDirectionDotNormal = r.GetDirection().dot(nl);


	//Calculate angle of ray relative to surface.
	double cosine2Theta = 1 - nnt * nnt * (1 - rayDirectionDotNormal * rayDirectionDotNormal);

	/*When the angle of incident is greater than some value called the critical angle, 
	then 100 % of the light incident on the surface is reflected.In another words, 
	when the angle of incident is greater than the critical angle, there isn't any refraction at all.
	This only happens though when the light ray passes from one medium to another medium with a lower index of refraction, 
	such as in the case of a water-air, diamond-water or glass-water interaction. This phenomenon is called ...

	"total internal reflection."	*/
	if(cosine2Theta < 0)
	{
		objects.clear();
		bvh->intersectTree(reflRay, bvh->thisNode.get(), objects);
		return hitObj->emission + objectColour.multiplyBy(radiance(reflRay, depth, xSubi, objects, bvh));
	}
		
	Vector3 tdir;
	if(into)
	{
		//fresnel term?
		tdir = r.GetDirection()*nnt - normal * ( 1 *(rayDirectionDotNormal*nnt + sqrt(cosine2Theta)));
	}
	else
	{
		//fresnel term?
		tdir = r.GetDirection()*nnt - normal * ( -1 * (rayDirectionDotNormal*nnt + sqrt(cosine2Theta)));
	}
	tdir.normalize();

	double a = glassRefractionIndex - airRefractionIndex; // == 1.5 - 1 == 0.5
	double b = glassRefractionIndex + airRefractionIndex; // == 1.5 + 1 = 2.5
	double reflectanceAtNormalIncidence = (a * a) / (b*b); // == 0.5^2 / (b^2)

	double c;	// c = 1 - cos(theta)
	if(into)
	{
		c = 1 - (-rayDirectionDotNormal);
	}
	else
	{
		c = tdir.dot(normal);
	}

	double fresnelReflectance = reflectanceAtNormalIncidence + (1 - reflectanceAtNormalIncidence)*c*c*c*c*c;
	double Tr = 1 - fresnelReflectance;
	double probabilityOfReflecting = .25 + .5*fresnelReflectance;
	double probabilityOfRefracting = fresnelReflectance / probabilityOfReflecting;
	double TP = Tr / (1 - probabilityOfReflecting);



	if(depth > 2)
	{
		if(erand48(xSubi) < probabilityOfReflecting)
		{
			objects.clear();
			bvh->intersectTree(reflRay, bvh->thisNode.get(), objects);

			return hitObj->emission + objectColour.multiplyBy(radiance(reflRay, depth, xSubi, objects, bvh)*probabilityOfRefracting);
		}
		else
		{
			Ray ray(hitPoint, tdir);
			objects.clear();
			bvh->intersectTree(ray, bvh->thisNode.get(), objects);

			return hitObj->emission + objectColour.multiplyBy(radiance(ray, depth, xSubi, objects, bvh)*TP);
		}
	}
	else
	{
		Ray ray2(hitPoint, tdir);

		objects.clear();
		bvh->intersectTree(reflRay, bvh->thisNode.get(), objects);
		bvh->intersectTree(ray2, bvh->thisNode.get(), objects);
		return hitObj->emission + objectColour.multiplyBy((radiance(reflRay, depth, xSubi, objects, bvh) * fresnelReflectance + radiance(ray2, depth, xSubi, objects, bvh)*Tr) );
	}
}



void threadOver(int samples, int yStart, int yEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera _cam, std::atomic<int>* _counter, std::string* _string, BoundingVolumeHierarchy* _bvh)
{
	Vector3 cx = _cam.GetRightFOVAdjusted();
	//preserve aspect ration based on camRight
	Vector3 cy = _cam.GetUpFOVAdjusted();

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

						std::vector<Object*> objectsHit;

						Ray ray(_cam.position, direction.normalize());

						_bvh->intersectTree(ray, _bvh->thisNode.get(), objectsHit);

						r = r + radiance(Ray(_cam.position, direction.normalize()), 0, Xi, objectsHit, _bvh) * (1. / samples);
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
	int width = 640, height = 480, samps = argc == 2 ? atoi(argv[1]) / 4 : 1; // # samples 

	BoundingVolumeHierarchy bvh;

	std::vector<Object*> objectsVector;

	//TODO -remove the hard coded spheres array and use a vector so we can "get size" here and scale it
	for(int i = 0; i <12; i++)
	{
		objectsVector.push_back(&spheres[i]);
	}

	bvh.BuildBVH(bvh.thisNode.get(), objectsVector);

	samps = 8; //override sample!

	//fixed FOV to be in degrees. modifies the length of the camRight vector by using tan(fov/2)
	double fov = 90;

	Camera cam(Vector3(0, 52, -240), Vector3(0, 10, -1), width, height, fov); // cam pos, position to look at
	
	bool threaded = true;
	bool useBVH = true;

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

		std::thread thread = std::thread(threadOver, samps, 0, height / 4, width, height, &pixelColour, cam, &finishedThreads, &t1, &bvh);
		std::thread thread2 = std::thread(threadOver, samps, height / 4, (height / 4) * 2, width, height, &pixelColour, cam, &finishedThreads, &t2, &bvh);
		std::thread thread3 = std::thread(threadOver, samps, (height / 4) * 2, (height / 4) * 3, width, height, &pixelColour, cam, &finishedThreads, &t3, &bvh);
		std::thread thread4 = std::thread(threadOver, samps, (height / 4) * 3, height, width, height, &pixelColour, cam, &finishedThreads, &t4, &bvh);

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
		Vector3 cx = cam.GetRightFOVAdjusted();
		Vector3 cy = cam.GetUpFOVAdjusted();

		Vector3 totalRadiance;

		if(useBVH)
		{
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
						for(int subPixelX = 0; subPixelX<2; subPixelX++, totalRadiance = Vector3()) // 2x2 subpixel cols 
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

								std::vector<Object*> objectsHit;

								Ray ray(cam.position, direction.normalize());

								bvh.intersectTree(ray, bvh.thisNode.get(), objectsHit);

								//push the offsets on the camera's "forward"
								totalRadiance = totalRadiance + radiance( Ray(cam.position, direction.normalize() ), 0, Xi, objectsHit, &bvh)*(1. / samps );

								objectsHit.clear();
							}
							pixelColour[i] = pixelColour[i] + Vector3(clamp(totalRadiance.x), clamp(totalRadiance.y), clamp(totalRadiance.z))*.25;
						}
					}
				}
			}
		}
		else
		{
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
						for(int subPixelX = 0; subPixelX<2; subPixelX++, totalRadiance = Vector3()) // 2x2 subpixel cols 
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

								std::vector<Object*> objectsHit;

								Ray ray(cam.position, direction.normalize() );

								bvh.intersectTree(ray, bvh.thisNode.get(), objectsHit);

								//push the offsets on the camera's "forward"
								totalRadiance = totalRadiance + radiance(Ray(cam.position, direction.normalize()), 0, Xi, objectsHit, &bvh)*(1. / samps);
							}
							pixelColour[i] = pixelColour[i] + Vector3(clamp(totalRadiance.x), clamp(totalRadiance.y), clamp(totalRadiance.z))*.25;
						}
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

